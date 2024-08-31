/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

using Symmetry.Console;
using Symmetry.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Symmetry.Server.NetMessages
{
    class NetMsgSndFile : NetworkMessage
    {
        private SvnFile File;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \fn public NetMsgSndFile(SvnFile file)
        ///
        /// \brief  Constructor.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public NetMsgSndFile(SvnFile file)
        {
            this.File = file;
        }

        public override void Send(BinaryReader r, BinaryWriter w, Client client)
        {
            // Send packet type
            w.Write((short)NetMsg.SendFile);

            // We want to copy the files contents into a new buffer because
            // if the client wants the file again, we don't want to encrypt
            // the data twice.
            byte[] data = new byte[this.File.Data.Length];
            this.File.Data.CopyTo(data, 0);

            // Perform encryption
            if (this.File.IsEncrypted)
            {
                using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
                {
                    aes.Key = client.AES.Key;
                    aes.IV = client.AES.IV;

                    aes.Padding = PaddingMode.PKCS7;
                    aes.Mode = CipherMode.CBC;

                    using (MemoryStream msEncrypt = new MemoryStream())
                    {
                        using (ICryptoTransform encryptor = aes.CreateEncryptor(aes.Key, aes.IV))
                        {
                            using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                            {
                                csEncrypt.Write(data, 0, data.Length);
                                csEncrypt.FlushFinalBlock();
                                data = msEncrypt.ToArray();
                            }
                        }
                    }
                }
            }

            // Calculate relative path
            int pathlen = Properties.Settings.Default.SvnPath.Length;
            string path = this.File.Path.Substring(pathlen, this.File.Path.Length - pathlen);

            // Remove project folder from path
            int projlength = path.Split(new char[] { '\\', '/' }).First().Length + 1;
            path = path.Substring(projlength, path.Length - projlength);

            // Send hashes (injector file's real names are sent)
            if (this.File.IsInjectorFile)
            {
                w.Write(path);
            }
            else 
                w.Write(Util.Crypto.SHA256.Compute(path));
            
            w.Write(Util.Crypto.SHA256.Compute(data));

            // Then some important traits
            w.Write(this.File.IsEncrypted);
            w.Write(this.File.IsMapped);
            w.Write(this.File.IsLuaAutorun);
            w.Write(this.File.IsInjectorFile);

            // And finally the actual data
            w.Write(data.Length);
            w.Write(data);

            // The client should send back a boolean stating that the file was downloaded properly, if it wasn't
            // we'll resend the file
            if (!r.ReadBoolean())
                this.Send(r, w, client);
        }
    }
}
