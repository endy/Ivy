using System;

namespace Ivy
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            using (IvyGame game = new IvyGame())
            {
                game.Run();
            }
        }
    }
}

