using System;
using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace RejectionSampling
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// This program demonstartes rejection sampling for a function.
    /// It generates 10000 samples, and plots a graph.
    /// The x axis represents x values for the function.
    /// The y axis represents the density of points in unit x.
    /// </summary>
    public partial class MainWindow : Window
    {
        ArrayList arrayList, densityList;
        float m = 0, fl = 0;
        float w = 50;
        int numberOfSamples = 10000;
        public MainWindow()
        {
            InitializeComponent();
            rejectionSampling();
            densityList = new ArrayList();
            for (int i = 0; i < arrayList.Count; i++)
            {
                m++;
                PointF p = arrayList[i] as PointF;
                if (Math.Floor(p.x) > fl)
                {
                    fl = (float)Math.Floor(p.x);
                    densityList.Add(m);
                    m = 0;
                }
            }

            Canvas c = canvas as Canvas;
            for (int i = 0; i < densityList.Count; i++)
            {
                Rectangle r = new Rectangle();
                r.Stroke = System.Windows.Media.Brushes.Black;
                r.Fill = System.Windows.Media.Brushes.SkyBlue;
                int y = Convert.ToInt32(densityList[i]);
                r.Height = y;
                r.Width = 10;
                Canvas.SetTop(r, 0);
                Canvas.SetLeft(r, i * 10);
                c.Children.Add(r);
            }

        }

        public void rejectionSampling()
        {
            arrayList = new ArrayList();
            Random rnd = new Random();
            int count = 0;
            for (; ; )
            {
                float x = (float)rnd.NextDouble() * w;
                float y = (float)rnd.NextDouble() * w;

                if (y <= f(x))
                {
                    Console.WriteLine(x + " " + y);
                    count++;
                    PointF p = new PointF(x, y);
                    arrayList.Add(p);
                    if (count >= numberOfSamples) { break; }
                }
            }
            IComparer sampleSorter = new SampleSorter();
            arrayList.Sort(sampleSorter);

        }

        public float f(float x)
        {
            return x;
        }
    }

    public class PointF
    {
        public float x, y;
        public PointF(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }

    public class SampleSorter : IComparer
    {
        public int Compare(Object a, Object b)
        {
            PointF a1 = a as PointF;
            PointF b1 = b as PointF;

            float x1 = a1.x;
            float x2 = b1.x;

            return x1.CompareTo(x2);

        }
    }
}
