using System;
using System.Collections.Generic;
using System.IO;

namespace Zju.Util
{
    /// <summary>
    /// 
    /// </summary> 
    public class FileProperties : Dictionary<string, string>
    {
        private List<string> keys = new List<string>();

        // Properties file name
        private String fileName = string.Empty;
        /// <summary>
        ///
        /// </summary>
        /// <param name="fileName">file name</param>
        public FileProperties(string fileName)
        {
            this.fileName = fileName;
            Load(fileName);
        }

        /// <summary>
        /// override Add, the Keys will sort by added
        /// </summary>
        /// <param name="key">key</param>

        ///<param name="value">value</param>
        /// <returns></returns>
        public void Add(string key, string value)
        {
            base.Add(key, value);
            keys.Add(key);
        }

        public List<string> Keys
        {
            get
            {
                return keys;
            }
        }

        /// <summary>
        /// load property file.
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        public void Load(string filePath)
        {
            char[] convertBuf = new char[1024];

            int limit;
            int keyLen;
            int valueStart;
            char c;
            string bufLine = string.Empty;
            bool hasSep;
            bool precedingBackslash;

            using (StreamReader sr = new StreamReader(filePath))
            {
                while (sr.Peek() >= 0)
                {
                    bufLine = sr.ReadLine();
                    limit = bufLine.Length;
                    keyLen = 0;
                    valueStart = limit;
                    hasSep = false;

                    precedingBackslash = false;
                    if (bufLine.StartsWith("#"))
                        keyLen = bufLine.Length;

                    while (keyLen < limit)
                    {
                        c = bufLine[keyLen];
                        if ((c == '=' || c == ':') & !precedingBackslash)
                        {
                            valueStart = keyLen + 1;
                            hasSep = true;
                            break;
                        }
                        else if ((c == ' ' || c == '\t' || c == '\f') & !precedingBackslash)
                        {
                            valueStart = keyLen + 1;
                            break;
                        }
                        if (c == '\\')
                        {
                            precedingBackslash = !precedingBackslash;
                        }
                        else
                        {
                            precedingBackslash = false;
                        }
                        keyLen++;
                    }

                    while (valueStart < limit)
                    {
                        c = bufLine[valueStart];
                        if (c != ' ' && c != '\t' && c != '\f')
                        {
                            if (!hasSep && (c == '=' || c == ':'))
                            {
                                hasSep = true;
                            }
                            else
                            {
                                break;
                            }
                        }
                        valueStart++;
                    }

                    string key = bufLine.Substring(0, keyLen);

                    string values = bufLine.Substring(valueStart, limit - valueStart);

                    if (key == "")
                        key += "#";
                    while (key.StartsWith("#") & this.ContainsKey(key))
                    {
                        key += "#";
                    }

                    this.Add(key, values);
                }
            }
        }

        /// <summary>
        /// save properties file.
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        public void Save(string filePath)
        {
            if (File.Exists(filePath))
            {
                File.Delete(filePath);
            }
            FileStream fileStream = File.Create(filePath);
            StreamWriter sw = new StreamWriter(fileStream);
            foreach (string key in keys)
            {
                String val = this[key];
                if (key.StartsWith("#"))
                {
                    if (val == "")
                    {
                        sw.WriteLine(key);
                    }
                    else
                    {
                        sw.WriteLine(val);
                    }
                }
                else
                {
                    sw.WriteLine(key + "=" + val);
                }
            }
            sw.Close();
            fileStream.Close();
        }
    }
}