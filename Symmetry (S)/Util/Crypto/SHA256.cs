/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Util.Crypto
{
    class SHA256
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string Compute(byte[] input)
        ///
        /// \brief  Computes the SHA256 hash of the byte array.
        ///
        /// \param  input   The data to hash.
        ///
        /// \return The hash of the data.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string Compute(byte[] input)
        {
            using (SHA256Managed sha = new SHA256Managed())
            {
                byte[] raw = sha.ComputeHash(input);
                StringBuilder build = new StringBuilder();

                foreach (byte b in raw)
                    build.Append(b.ToString("x2"));

                return build.ToString();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public static string Compute(string input)
        ///
        /// \brief  Overload for above, to take string input.
        ///
        /// \param  input   The input.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string Compute(string input)
        {
            return Compute(Encoding.UTF8.GetBytes(input));
        }
    }
}
