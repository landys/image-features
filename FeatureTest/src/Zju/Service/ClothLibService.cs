using System;
using System.Collections;
using System.Collections.Generic;
using Zju.Domain;
using Zju.Dao;
using Zju.Util;
using System.Threading;


namespace Zju.Service
{
    /// <summary>
    /// It is thread safe.
    /// </summary>
    public class ClothLibService : IClothLibService
    {
        private IClothDao clothDao;

        public ClothLibService()
        {
            this.clothDao = new ClothDao();
        }

        #region IClothLibService Members

        public void Insert(Cloth cloth)
        {
            if (cloth.Oid > 0 || string.IsNullOrEmpty(cloth.Path))
            {
                return;
            }

            //Cloth existCloth = clothDao.FindByPath(cloth.Path);
            

            if (0 == cloth.UpdateTime.Ticks)
            {
                cloth.UpdateTime = DateTime.UtcNow;
            }
            clothDao.Insert(cloth);
        }

        private void InsertAll(List<Cloth> clothes)
        {
            foreach (Cloth cloth in clothes)
            {
                if (0 == cloth.UpdateTime.Ticks)
                {
                    cloth.UpdateTime = DateTime.UtcNow;
                }
            }
            clothDao.InsertAll(clothes);
        }

        public void Update(Cloth cloth, Cloth newCloth)
        {
            clothDao.Update(cloth, newCloth);
        }

        public void Delete(int oid)
        {
            if (oid > 0)
            {
                clothDao.Delete(oid);
            }
        }

        public void Delete(Cloth cloth)
        {
            clothDao.Delete(cloth);
        }

        public List<Cloth> FindAll()
        {
            return clothDao.FindAll();
        }


        public void ModifyClothPicPath(string origPath, string modPath)
        {
            List<Cloth> clothes = clothDao.FindAll();
            List<Cloth> lc = new List<Cloth>();
            foreach (Cloth cloth in clothes)
            {
                string s = cloth.Path.Replace(origPath, modPath);
                if (!cloth.Path.Equals(s))
                {
                    cloth.Path = s;
                    lc.Add(cloth);
                    if (lc.Count == 100)
                    {
                        clothDao.UpdatePathAll(lc);
                        lc.Clear();
                    }
                }
            }
            if (lc.Count > 0)
            {
                clothDao.UpdatePathAll(lc);
            }
        }

        /// <summary>
        /// If use multithread, than gabor featrue will not be extracted.
        /// </summary>
        /// <param name="argus"></param>
        public void AsynImportClothPics(ImportArgus argus)
        {
            if (argus.PicNames == null || argus.PicNames.Count == 0)
            {
                return;
            }

            if (argus.PicNames.Count < argus.ThreadNum)
            {
                argus.ThreadNum = argus.PicNames.Count;
            }
            else if (argus.ThreadNum <= 0)
            {
                argus.ThreadNum = 1;
            }
            else if (argus.ThreadNum > SearchConstants.MAX_IMPORT_THREADS)
            {
                argus.ThreadNum = SearchConstants.MAX_IMPORT_THREADS;
            }

            ParameterizedThreadStart threadDelegate = new ParameterizedThreadStart(importClothPics);
            Thread importPicsThread = new Thread(threadDelegate);
            importPicsThread.IsBackground = true;
            importPicsThread.Start(argus);
        }

        private void importClothPics(Object obj)
        {
            ImportArgus argus = (ImportArgus)obj;
            List<String> picNames = argus.PicNames;

            // show progress win
            argus.BefDel(picNames.Count);

            Queue picNameQueue = Queue.Synchronized(new Queue(picNames));

            // finished pictures
            //nFinished = 0;

            ParameterizedThreadStart threadDelegate = new ParameterizedThreadStart(importClothPicThread);
            Thread[] threads = new Thread[argus.ThreadNum];
            bool isGabor = (argus.ThreadNum == 1);
            for (int i = 0; i < threads.Length; ++ i )
            {
                threads[i] = new Thread(threadDelegate);
                threads[i].IsBackground = true;
                threads[i].Priority = ThreadPriority.BelowNormal;
                threads[i].Start(new ImportThreadArgus(picNameQueue, argus.Step, argus.StopImport, argus.StepDel, isGabor));
            }

            for (int i = 0; i < threads.Length; ++i)
            {
                threads[i].Join();
            }

            argus.AfterDel();
        }

        private void importClothPicThread(Object obj)
        {
            ImportThreadArgus argus = (ImportThreadArgus)obj;

            List<Cloth> clothes = new List<Cloth>(argus.Step);

            while (!argus.StopImport())
            {
                String picName = dequeuePicName(argus.PicNameQueue);
                if (picName == null)
                {
                    break;
                }
                Cloth cloth = ClothUtil.GenerateClothObject(picName, argus.isGabor);

                clothes.Add(cloth);
                if (clothes.Count % argus.Step == 0)
                {
                    InsertAll(clothes);
                    argus.StepDel(clothes.Count);
                    clothes.Clear();
                }

            }
            if (clothes.Count > 0)
            {
                InsertAll(clothes);
                argus.StepDel(clothes.Count);
            }

        }

        private String dequeuePicName(Queue picQueue)
        {
            String name = null;
            try
            {
                while (String.IsNullOrEmpty(name))
                {
                    name = (String)picQueue.Dequeue();
                }
            }
            catch (System.InvalidOperationException e)
            {
                // empty queue.
                return null;
            }

            return name;
        }

        private class ImportThreadArgus
        {
            internal Queue PicNameQueue;
            internal IntArgDelegate StepDel;
            internal ShouldStop StopImport;
            internal int Step;
            internal bool isGabor;

            internal ImportThreadArgus(Queue picNameQueue, int step,
                ShouldStop stopImport, IntArgDelegate stepDel, bool isGabor)
            {
                this.PicNameQueue = picNameQueue;
                this.Step = step;
                this.StopImport = stopImport;
                this.StepDel = stepDel;
                this.isGabor = isGabor;
            }
        }


        #endregion

        public IClothDao ClothDao
        {
            set { this.clothDao = value; }
        }
    }

    // TODO update not implemented yet.
    public class ImportArgus
    {
        // for import, picture file names, can be absolute path or relative path
        public List<String> PicNames;
        // for update, each Cloth should be already "Persistent".
        public List<Cloth> Clothes;
        // for bat commit, db commit until "Step" clothes inserted.
        public int Step;
        // thread number to import/update clothes
        public int ThreadNum;
        // callback functions
        public ShouldStop StopImport;
        public IntArgDelegate BefDel;
        public IntArgDelegate StepDel;
        public NoArgDelegate AfterDel;
        // mark if import or update
        public bool Update;

        public ImportArgus(List<String> picNames, List<Cloth> clothes, int step, int threadNum, ShouldStop stopImport,
            IntArgDelegate befDel, IntArgDelegate stepDel, NoArgDelegate afterDel, bool update)
        {
            this.PicNames = picNames;
            this.Clothes = clothes;
            this.Step = step;
            this.ThreadNum = threadNum;
            this.StopImport = stopImport;
            this.BefDel = befDel;
            this.StepDel = stepDel;
            this.AfterDel = afterDel;
            this.Update = update;
        }

        public ImportArgus(List<String> picNames, int step, int threadNum, ShouldStop stopImport,
            IntArgDelegate befDel, IntArgDelegate stepDel, NoArgDelegate afterDel) 
            : this(picNames, null, step, threadNum, stopImport, befDel, stepDel, afterDel, false)
        {
        }

        public ImportArgus(List<Cloth> clothes, int step, int threadNum, ShouldStop stopImport,
            IntArgDelegate befDel, IntArgDelegate stepDel, NoArgDelegate afterDel)
            : this(null, clothes, step, threadNum, stopImport, befDel, stepDel, afterDel, true)
        {
        }
    }

    public delegate void IntArgDelegate(int argi);

    public delegate void NoArgDelegate();

    public delegate bool ShouldStop();
}
