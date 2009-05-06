﻿using System;
using System.Windows;

namespace FeatureTest
{
    /// <summary>
    /// Interaction logic for IndeterminateProgressWin.xaml
    /// </summary>
    public partial class IndeterminateProgressWin : Window
    {
        public IndeterminateProgressWin(String title, String hint)
        {
            InitializeComponent();

            this.Title = title;
            lblProgSummary.Content = hint;
        }
    }
}
