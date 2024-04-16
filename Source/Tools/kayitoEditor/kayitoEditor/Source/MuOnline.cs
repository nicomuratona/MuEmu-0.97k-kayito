using kayito_Editor.Source;
using System;
using System.Data.OleDb;
using System.Windows.Forms;

namespace kayito_Editor
{
	class MuOnline
	{
		public MuOnline()
		{
			string connection_string;

			try
			{
				if (Import.MU_TRUSTED != 1)
				{
					connection_string = $"Provider=SQLOLEDB.1;Data Source={Import.MU_SERVER},{Import.MU_PORT};Initial Catalog={Import.MU_DB};Uid={Import.MU_DB_USER};Pwd={Import.MU_DB_PASS}";
				}
				else
				{
					connection_string = $"Provider=SQLOLEDB.1;Data Source={Import.MU_SERVER};Initial Catalog={Import.MU_DB};Integrated Security=SSPI;";
				}

				Import.Mu_Connection = new OleDbConnection(connection_string);

				Import.Mu_Connection.Open();

				if (Import.USE_ME == 1)
				{
					if (Import.ME_TRUSTED != 1)
					{
						connection_string = $"Provider=SQLOLEDB.1;Data Source={Import.ME_SERVER},{Import.ME_PORT};Initial Catalog={Import.ME_DB};Uid={Import.ME_DB_USER};Pwd={Import.ME_DB_PASS}";
					}
					else
					{
						connection_string = $"Provider=SQLOLEDB.1;Data Source={Import.ME_SERVER};Initial Catalog={Import.ME_DB};Integrated Security=SSPI;";
					}

					Import.Me_Connection = new OleDbConnection(connection_string);

					Import.Me_Connection.Open();
				}
				else
				{
					Import.Me_Connection = Import.Mu_Connection;
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);

				Environment.Exit(0);
			}
		}

		public static bool Mu_ExecuteSQL(string query)
		{
			bool flag = false;

			try
			{
				OleDbCommand cmd = new OleDbCommand(query, Import.Mu_Connection);

				cmd.ExecuteNonQuery();

				cmd.Dispose();

				flag = true;
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

				flag = false;
			}

			return flag;
		}

		public static bool Me_ExecuteSQL(string query)
		{
			bool flag = false;

			try
			{
				OleDbCommand cmd = new OleDbCommand(query, Import.Me_Connection);

				cmd.ExecuteNonQuery();

				cmd.Dispose();

				flag = true;
			}
			catch (Exception exception)
			{
				MessageBox.Show($"[SQL] {query}\n[Error] {exception.Message}\n[Source] {exception.Source}\n[Trace] {exception.StackTrace}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

				flag = false;
			}

			return flag;
		}

		public static void CloseConnection()
		{
			Import.Mu_Connection.Close();

			if (Import.USE_ME == 1)
			{
				Import.Me_Connection.Close();
			}
		}
	}
}