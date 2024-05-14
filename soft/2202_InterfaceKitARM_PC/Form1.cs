using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO.Ports;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;

using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace _2202_InterfaceKitARM_PC
{
    public partial class FormKitArm : Form
    {
        private PictureBox[] LCD_Char;

        public FormKitArm()
        {
            InitializeComponent();

            int LCD_Size = 40;

            LCD_Char = new PictureBox[LCD_Size];

            for (int i = 0; i < LCD_Size; i++)
            {
                LCD_Char[i] = (PictureBox)Controls.Find("LCD" + i, true)[0];
            }

            box_USB.Items.AddRange(SerialPort.GetPortNames());
        }

        private void LED_CheckedChanged(object sender, EventArgs e)
        {
            CheckBox LED = sender as CheckBox;

            if (LED.Checked)
            {
                LED.BackgroundImage = Properties.Resources.led_allumee;
            }
            else
            {
                LED.BackgroundImage = Properties.Resources.led_eteinte;
            }
        }
        private void BTN_MouseDown(object sender, EventArgs e)
        {
            Button BTN = sender as Button;

            BTN.BackgroundImage = Properties.Resources.button_pressed;
        }

        private void BTN_Click(object sender, EventArgs e)
        {
            Button BTN = sender as Button;

            BTN.BackgroundImage = Properties.Resources.button_release;
        }

        private void LCD_DisplayChar(char myChar, int position)
        {
                /* Convert caracter to ASCII value then to hexadecimal */
                string imageName = "_" + ((int)myChar).ToString();

                /* If image not found, display a space instead */
                if (Properties.Resources.ResourceManager.GetObject(imageName) == null)
                {
                    imageName = "_32";
                }

                LCD_Char[position].Image = (Image)Properties.Resources.ResourceManager.GetObject(imageName);
        }

        private void btn_USB_Click(object sender, EventArgs e)
        {
            if ((Port_USB.PortName != box_USB.Text) && (box_USB.Text != ""))
            {
                if (Port_USB.IsOpen)
                    Port_USB.Close();

                Port_USB.PortName = box_USB.Text;

                Port_USB.Open();

                image_USB.Image = Properties.Resources.USB_Connected;
                
            }
        }

        private void LCD_Line1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string strLine = LCD_Line1.Text;
                for (int i = 0; i < strLine.Length; i++)
                    LCD_DisplayChar(strLine[i], i);
            }
        }

        private void LCD_Line2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                string strLine = LCD_Line2.Text;
                for (int i = 0; i < strLine.Length; i++)
                    LCD_DisplayChar(strLine[i], i + 20);
            }
        }
    }
}
