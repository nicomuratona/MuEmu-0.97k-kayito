using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace kayito_Editor.Source
{
	class MemScript
	{
		char[] m_buff;

		int m_size = 0;

		string m_path;

		int m_count;

		float m_number;

		char[] m_string = new char[256];

		string m_LastError;

		private readonly string MEM_SCRIPT_ALLOC_ERROR = "Could not alloc memory for MemScript\n";

		private readonly string MEM_SCRIPT_ERROR_CODE0 = "{0} Could not open file\n";

		private readonly string MEM_SCRIPT_ERROR_CODE1 = "{0} Could not alloc file buffer\n";

		private readonly string MEM_SCRIPT_ERROR_CODE2 = "{0} Could not read file\n";

		private readonly string MEM_SCRIPT_ERROR_CODE3 = "{0} Could not get file buffer\n";

		private readonly string MEM_SCRIPT_ERROR_CODE4 = "{0} The file were not configured correctly\n";

		private readonly string MEM_SCRIPT_ERROR_CODEX = "{0} Unknow error code: {1}\n";

		public enum eTokenResult
		{
			TOKEN_NUMBER = 0,
			TOKEN_STRING = 1,
			TOKEN_END = 2,
			TOKEN_ERROR = 3,
		};

		public MemScript()
		{
			this.m_LastError = this.MEM_SCRIPT_ALLOC_ERROR;
		}

		public bool SetBuffer(string path)
		{
			this.m_path = path;

			this.SetLastError(4);

			try
			{
				if (!File.Exists(path))
				{
					throw new NullReferenceException(path + " not found.");
				}

				StreamReader reader = new StreamReader(path, Encoding.Default);

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

		char CheckComment(char ch)
		{
			if (ch != '/' || (ch = this.GetChar()) != '/')
			{
				return ch;
			}

			while (true)
			{
				if ((ch = this.GetChar()) == (char)0xFF)
				{
					return ch;
				}

				if (ch == '\n')
				{
					return ch;
				}
			}

			return ch;
		}

		public eTokenResult GetToken()
		{
			this.m_number = 0;

			this.m_string = new char[256];

			char ch;

			while (true)
			{
				if ((ch = this.GetChar()) == (char)0xFF)
				{
					return eTokenResult.TOKEN_END;
				}

				if (Char.IsWhiteSpace(ch) || ch == '\t')
				{
					continue;
				}

				if ((ch = this.CheckComment(ch)) == (char)0xFF)
				{
					return eTokenResult.TOKEN_END;
				}
				else if (ch != '\n')
				{
					break;
				}
			}

			if (ch == '-' || ch == '+' || ch == '.' || ch == '*' || Char.IsDigit(ch))
			{
				return this.GetTokenNumber(ch);
			}

			if (ch == '"')
			{
				return this.GetTokenString();
			}

			return this.GetTokenCommon(ch);
		}

		eTokenResult GetTokenNumber(char ch)
		{
			int count = 0;

			this.UnGetChar(ch);

			while ((ch = this.GetChar()) != (char)0xFF && (ch == '-' || ch == '.' || ch == '*' || Char.IsDigit(ch)))
			{
				this.m_string[count++] = ch;
			}

			string checkString = new string(this.m_string).Trim();

			if (checkString.Equals("*"))
			{
				this.m_number = -1;
			}
			else
			{
				this.m_number = (float)this.MyAtof(this.m_string);
			}

			this.m_string[count] = (char)0;

			return eTokenResult.TOKEN_NUMBER;
		}

		eTokenResult GetTokenString()
		{
			char ch;

			int count = 0;

			while ((ch = this.GetChar()) != (char)0xFF && ch != '"')
			{
				this.m_string[count++] = ch;
			}

			if (ch != '"')
			{
				this.UnGetChar(ch);
			}

			this.m_string[count] = (char)0;

			return eTokenResult.TOKEN_STRING;
		}

		eTokenResult GetTokenCommon(char ch)
		{
			if (!Char.IsLetter(ch))
			{
				return eTokenResult.TOKEN_ERROR;
			}

			int count = 0;

			this.m_string[count++] = ch;

			while ((ch = this.GetChar()) != (char)0xFF && (ch == '.' || ch == '_' || Char.IsLetter(ch) || Char.IsDigit(ch)))
			{
				this.m_string[count++] = ch;
			}

			this.UnGetChar(ch);

			this.m_string[count] = (char)0;

			return eTokenResult.TOKEN_STRING;
		}

		private void SetLastError(int error)
		{
			switch (error)
			{
				case 0:
				{
					this.m_LastError = string.Format(this.MEM_SCRIPT_ERROR_CODE0, m_path);

					break;
				}

				case 1:
				{
					this.m_LastError = string.Format(this.MEM_SCRIPT_ERROR_CODE1, m_path);

					break;
				}

				case 2:
				{
					this.m_LastError = string.Format(this.MEM_SCRIPT_ERROR_CODE2, m_path);

					break;
				}

				case 3:
				{
					this.m_LastError = string.Format(this.MEM_SCRIPT_ERROR_CODE3, m_path);

					break;
				}

				case 4:
				{
					this.m_LastError = string.Format(this.MEM_SCRIPT_ERROR_CODE4, m_path);

					break;
				}

				default:
				{
					this.m_LastError = string.Format(this.MEM_SCRIPT_ERROR_CODEX, m_path, error);

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
			return (int)this.m_number;
		}

		public int GetAsNumber()
		{
			this.GetToken();

			return (int)this.m_number;
		}

		public float GetFloatNumber()
		{
			return this.m_number;
		}

		public float GetAsFloatNumber()
		{
			this.GetToken();

			return this.m_number;
		}

		public string GetString()
		{
			return new string(this.m_string).Trim();
		}

		public string GetAsString()
		{
			this.GetToken();

			return new string(this.m_string).Trim();
		}

		private bool testChar(char c)
		{
			return (c == '.' || c == '-' || c == '+' || char.IsDigit(c));
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