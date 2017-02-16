using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab1
{
    static class GameManager
    {
        private static readonly Farm farm;

    
        static GameManager()
        {
            farm = Farm.Instance;
        }

        public static void RunGame()
        {
            // create base gameObjects and wait for users actions

            for (int i = 0; i < 52; i++)
            {
                try
                {
                    farm.storage.AddItem(new Carrot());
                }
                catch (StorageOverflowException ex)
                {
                    System.Console.WriteLine(ex.Message);
                }
            }

            try
            {
                List<GameItem> l = new List<GameItem>();
                for (var j = 0; j < 12; j++)
                    l.Add(new Carrot());
                farm.storage.AddItems(l);
            }
            catch (StorageOverflowException ex)
            {
                System.Console.WriteLine(ex.Message);
            }
        }
    }
}
