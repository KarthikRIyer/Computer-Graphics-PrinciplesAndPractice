using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Ch5_ex2_motion_induced_blindness
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        int t = 10;
        RotateTransform rotateCanvasTransform;
        public MainWindow()
        {
            InitializeComponent();
            // Window window = new Window();

            Canvas canvas = this.FindName("canvas") as Canvas;
            canvas.Background = Brushes.Transparent;
            Canvas canvas2 = this.FindName("canvas2") as Canvas;
            canvas2.Background = Brushes.Transparent;

            Ellipse ec = new Ellipse();
            SolidColorBrush brush2 = new SolidColorBrush();
            brush2.Color = Colors.Yellow;
            ec.Stroke = Brushes.Yellow;
            ec.Fill = brush2;
            ec.StrokeThickness = 2;
            ec.Height = 8;
            ec.Width = 8;
            ec.RenderTransformOrigin = new Point(0, 0);
            TranslateTransform translateTransformc = new TranslateTransform();
            translateTransformc.X = 318;
            translateTransformc.Y = 174;
            TransformGroup transformGroupc = new TransformGroup();
            transformGroupc.Children.Add(translateTransformc);
            ec.RenderTransform = transformGroupc;
            canvas2.Children.Add(ec);

            Ellipse e1 = new Ellipse();
            e1.Stroke= Brushes.Yellow;
            e1.Fill = brush2;
            e1.StrokeThickness= 2;
            e1.Height = 16;
            e1.Width = 16;
            e1.RenderTransformOrigin = new Point(0,0);
            TranslateTransform translateTransform1 = new TranslateTransform();
            translateTransform1.X = 318-5;
            translateTransform1.Y = 174-50;
            TransformGroup transformGroup1 = new TransformGroup();
            transformGroup1.Children.Add(translateTransform1);
            e1.RenderTransform = transformGroup1;
            canvas2.Children.Add(e1);

            Ellipse e2 = new Ellipse();
            e2.Stroke = Brushes.Yellow;
            e2.Fill = brush2;
            e2.StrokeThickness = 2;
            e2.Height = 16;
            e2.Width = 16;
            e2.RenderTransformOrigin = new Point(0, 0);
            TranslateTransform translateTransform2 = new TranslateTransform();
            translateTransform2.X = 313 - 43;
            translateTransform2.Y = 174 + 25;
            TransformGroup transformGroup2 = new TransformGroup();
            transformGroup2.Children.Add(translateTransform2);
            e2.RenderTransform = transformGroup2;
            canvas2.Children.Add(e2);


            Ellipse e3 = new Ellipse();
            e3.Stroke = Brushes.Yellow;
            e3.Fill = brush2;
            e3.StrokeThickness = 2;
            e3.Height = 16;
            e3.Width = 16;
            e3.RenderTransformOrigin = new Point(0, 0);
            TranslateTransform translateTransform3 = new TranslateTransform();
            translateTransform3.X = 313 + 43;
            translateTransform3.Y = 174 + 25;
            TransformGroup transformGroup3 = new TransformGroup();
            transformGroup3.Children.Add(translateTransform3);
            e3.RenderTransform = transformGroup3;
            canvas2.Children.Add(e3);

            //Line l1, l2;
            //l1 = new Line();
            //l2 = new Line();
            //l1.StrokeThickness = 2;
            //l2.StrokeThickness = 2;

            SolidColorBrush brush = new SolidColorBrush();
            brush.Color = Colors.Blue;

            //l1.Stroke = brush;
            //l2.Stroke = brush;170 330

            int inc = 24;


            for (int x1 = 10-792, x3 = 0-792; x1 <= 800*1.5; x1 += inc, x3 += inc)
            {
                for (int y1 = 0-408, y3 = 10-408; y1 <= 450*1.5; y1 += inc, y3 += inc)
                {
                    Line l1 = new Line();
                    Line l2 = new Line();
                    l1.StrokeThickness = 2;
                    l2.StrokeThickness = 2;
                    l1.Stroke = brush;
                    l2.Stroke = brush;
                    l1.X1 = x1;
                    l1.Y1 = y1;
                    l1.X2 = x1;
                    l1.Y2 = y1 + 20;
                    l2.X1 = x3;
                    l2.Y1 = y3;
                    l2.X2 = x3 + 20;
                    l2.Y2 = y3;
                    canvas.Children.Add(l1);
                    canvas.Children.Add(l2);

                }
            }

            canvas.ClipToBounds = false;

            Storyboard storyboard = new Storyboard();
            storyboard.Duration = new Duration(TimeSpan.FromSeconds(t));

            DoubleAnimation rotateAnimation = new DoubleAnimation() {
                From = 0,
                To = 360,
                Duration = storyboard.Duration,
                RepeatBehavior = RepeatBehavior.Forever

            };
            rotateCanvasTransform = new RotateTransform();
            rotateCanvasTransform.Angle = 0;
            rotateCanvasTransform.CenterX = 318;
            rotateCanvasTransform.CenterY = 174;
            canvas.RenderTransform = rotateCanvasTransform;
            //Storyboard.SetTarget(rotateAnimation, canvas);
            //Storyboard.SetTargetProperty(rotateAnimation, new PropertyPath("(UIElement.RenderTransform).(RotateTransform.Angle)"));
            //storyboard.Children.Add(rotateAnimation);
            rotateCanvasTransform.BeginAnimation(RotateTransform.AngleProperty, rotateAnimation);
            
                
               

            //window.Content = canvas;
            //window.Title = "CanvasSample";
            //window.Show();


        }

        private void sliderChanged(object sender,RoutedPropertyChangedEventArgs<double> e) {
            t = (int)timeSlider.Value;
            Storyboard storyboard = new Storyboard();
            storyboard.Duration = new Duration(TimeSpan.FromSeconds(t));
            DoubleAnimation rotateAnimation = new DoubleAnimation()
            {
                From = 0,
                To = 360,
                Duration = storyboard.Duration,
                RepeatBehavior = RepeatBehavior.Forever

            };
            rotateCanvasTransform.BeginAnimation(RotateTransform.AngleProperty, rotateAnimation);
        }
        

    }
}
