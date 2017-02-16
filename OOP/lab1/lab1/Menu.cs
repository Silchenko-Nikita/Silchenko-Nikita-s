using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    public abstract class Menu : IDisplayable
    {
        private List<MenuItem> menuItems;
        private MenuItem statusLabel;
        private MenuButton exitButton;

        public Menu()
        {
            exitButton = new MenuButton();

            statusLabel.Label = "ACTIVE";
            exitButton.OnClickEvent += new OnClickHandle(this.ExitButtonClick);
        }

        public void Display()
        {
            int i = 0;
            foreach (MenuItem mI in menuItems)
            {
                //mI.setSize(smth, smth);
                //mI.SetPos(smth, smth(i));
                // display
                i++;
            }
            // display else
        }

        public void Hide()
        {
            // do hide   
        }

        public void ExitButtonClick(IClickable exitButton, OnClickEventArgs args)
        {
            statusLabel.Label = "CLOSING";
            // System.sleep(0.01) for example
            Hide();
        }
    }
}
