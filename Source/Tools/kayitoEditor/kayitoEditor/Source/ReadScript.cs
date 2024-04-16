using System;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace kayito_Editor.Source
{
	class ReadScript
	{
		private char[] m_buff;

		private int m_size = 0;

		private string m_path;

		private int m_count;

		private float m_number;

		private char[] m_string = new char[256];

		private int m_line;

		private eTokenResult m_lastToken;

		private string m_LastError;

		public enum eTokenResult
		{
			TOKEN_NUMBER = 0,
			TOKEN_STRING = 1,
			TOKEN_END = 2,
			TOKEN_END_LINE = 3,
			TOKEN_END_SECTION = 4,
			TOKEN_ERROR = 5,
		};

		public ReadScript()
		{
			this.m_LastError = $"[ReadScript] Could not alloc memory.\n";
		}

		public bool SetBuffer(string path)
		{
			this.m_path = path;

			this.SetLastError(4);

			try
			{
				if (!File.Exists(path))
				{
					this.SetLastError(0);

					return false;
				}

				StreamReader reader = new StreamReader(path);

				this.m_size = (int)reader.BaseStream.Length;

				this.m_buff = new char[this.m_size];

				if (this.m_buff.Length == 0)
				{
					this.SetLastError(1);

					return false;
				}

				if (reader.Read(this.m_buff, 0, this.m_size) == 0)
				{
					this.SetLastError(2);

					return false;
				}

				this.m_count = 0;

				this.m_line = 1;

				reader.Close();

				return true;
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);

				return false;
			}
		}

		char GetChar()
		{
			if (this.m_count >= this.m_size)
			{
				return (char)0xFF;
			}

			return this.m_buff[this.m_count++];
		}

		void UnGetChar(char ch)
		{
			if (this.m_count == 0)
			{
				return;
			}

			this.m_buff[--this.m_count] = ch;
		}

		public eTokenResult GetToken(bool wReturn = false)
		{
			this.m_number = -1;

			this.m_string = new char[256];

			char ch;

			do
			{
				if ((ch = this.GetChar()) == (char)0xFF)
				{
					this.m_lastToken = eTokenResult.TOKEN_END;

					return this.m_lastToken;
				}

				if (ch == '/')
				{
					if ((ch = this.GetChar()) == '/')
					{
						while ((ch != '\n') && (ch != (char)0xFF))
						{
							ch = this.GetChar();
						}

						if (ch == (char)0xFF)
						{
							this.m_lastToken = eTokenResult.TOKEN_END;

							return this.m_lastToken;
						}
					}
				}

				if (ch == '\n')
				{
					this.m_line++;

					if (wReturn)
					{
						this.m_lastToken = eTokenResult.TOKEN_END_LINE;

						return this.m_lastToken;
					}

					continue;
				}
			}
			while (char.IsWhiteSpace(ch));

			if (char.IsDigit(ch) || ch == '.' || ch == '-' || ch == '*')
			{
				int count = 0;

				this.UnGetChar(ch);

				while ((ch = this.GetChar()) != (char)0xFF && (char.IsDigit(ch) || ch == '.' || ch == '-' || ch == '*'))
				{
					this.m_string[count++] = ch;
				}

				this.m_string[count] = '\0';

				string checkString = new string(this.m_string).Trim('\0');

				if (checkString.Equals("*"))
				{
					this.m_number = -1;
				}
				else
				{
					this.m_number = this.MyAtof(this.m_string);
				}

				this.m_lastToken = eTokenResult.TOKEN_NUMBER;

				return this.m_lastToken;
			}
			else if (ch == '"')
			{
				int count = 0;

				while ((ch = this.GetChar()) != (char)0xFF && ch != '"')
				{
					this.m_string[count++] = ch;
				}

				if (ch != '"')
				{
					this.UnGetChar(ch);
				}

				this.m_string[count] = '\0';

				if (ch == (char)0xFF)
				{
					this.m_lastToken = eTokenResult.TOKEN_END;
				}
				else if (ch == '\n')
				{
					this.m_lastToken = eTokenResult.TOKEN_END_LINE;
				}
				else
				{
					this.m_lastToken = eTokenResult.TOKEN_STRING;
				}

				return this.m_lastToken;
			}
			else if (char.IsLetter(ch))
			{
				int count = 0;

				this.m_string[count++] = ch;

				while ((ch = this.GetChar()) != (char)0xFF && (ch == '.' || ch == '_' || char.IsLetter(ch) || char.IsDigit(ch)))
				{
					this.m_string[count++] = ch;
				}

				this.UnGetChar(ch);

				this.m_string[count] = '\0';

				string checkString = new string(this.m_string).Trim('\0');

				if (String.Compare(checkString, "end") == 0)
				{
					this.m_lastToken = eTokenResult.TOKEN_END_SECTION;
				}
				else if (ch == (char)0xFF)
				{
					this.m_lastToken = eTokenResult.TOKEN_END;
				}
				else
				{
					this.m_lastToken = eTokenResult.TOKEN_STRING;
				}

				return this.m_lastToken;
			}

			this.m_lastToken = eTokenResult.TOKEN_ERROR;

			return this.m_lastToken;
		}

		private void SetLastError(int error)
		{
			switch (error)
			{
				case 0:
				{
					this.m_LastError = $"[ReadScript] Could not open file '{this.m_path}'.\n";

					break;
				}

				case 1:
				{
					this.m_LastError = $"[ReadScript] Could not alloc file buffer '{this.m_path}'.\n";

					break;
				}

				case 2:
				{
					this.m_LastError = $"[ReadScript] Could not read file '{this.m_path}'.\n";

					break;
				}

				case 3:
				{
					this.m_LastError = $"[ReadScript] Could not get file buffer '{this.m_path}'.\n";

					break;
				}

				case 4:
				{
					this.m_LastError = $"[ReadScript] The file were not configured correctly '{this.m_path}'.\n";

					break;
				}

				case 5:
				{
					this.m_LastError = $"[ReadScript] There is an error in file '{this.m_path}' in line {this.m_line}.\n";

					break;
				}
			}
		}

		public string GetLastError()
		{
			return this.m_LastError;
		}

		public int GetNumber()
		{
			if (this.m_lastToken != eTokenResult.TOKEN_NUMBER)
			{
				this.SetLastError(5);

				throw new FormatException("File format is incorrect");
			}

			return (int)this.m_number;
		}

		public int GetAsNumber()
		{
			eTokenResult token = this.GetToken(true);

			if (token == eTokenResult.TOKEN_END_LINE || token != eTokenResult.TOKEN_NUMBER)
			{
				this.SetLastError(5);

				throw new FormatException("File format is incorrect");
			}

			return (int)this.m_number;
		}

		public float GetFloatNumber()
		{
			if (this.m_lastToken != eTokenResult.TOKEN_NUMBER)
			{
				this.SetLastError(5);

				throw new FormatException("File format is incorrect");
			}

			return this.m_number;
		}

		public float GetAsFloatNumber()
		{
			eTokenResult token = this.GetToken(true);

			if (token == eTokenResult.TOKEN_END_LINE || token != eTokenResult.TOKEN_NUMBER)
			{
				this.SetLastError(5);

				throw new FormatException("File format is incorrect");
			}

			return this.m_number;
		}

		public string GetString()
		{
			if (this.m_lastToken != eTokenResult.TOKEN_STRING)
			{
				this.SetLastError(5);

				throw new FormatException("File format is incorrect");
			}

			return new string(this.m_string).Trim();
		}

		public string GetAsString()
		{
			eTokenResult token = this.GetToken(true);

			if (token == eTokenResult.TOKEN_END_LINE || token != eTokenResult.TOKEN_STRING)
			{
				this.SetLastError(5);

				throw new FormatException("File format is incorrect");
			}

			return new string(this.m_string).Trim();
		}

		private bool testChar(char c)
		{
			return (char.IsDigit(c) || c == '.' || c == '-' || c == '+');
		}

		private float MyAtof(char[] myString)
		{
			string thisString = new string(myString);

			thisString = new string(thisString.Trim().TakeWhile(testChar).ToArray());

			if (thisString.Length > 0)
			{
				float rvl;

				if (float.TryParse(thisString, out rvl))
				{
					return rvl;
				}
			}

			return 0;
		}
	}
}