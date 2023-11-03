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
			Import.Mu_Connection = new OleDbConnection();

			string query;

			try
			{
				if (Import.MU_TRUSTED != 1)
				{
					object[] Params = new object[] { Import.MU_SERVER, Import.MU_PORT, Import.MU_DB, Import.MU_DB_USER, Import.MU_DB_PASS };

					query = string.Format("Provider=SQLOLEDB.1;Data Source={0},{1};Initial Catalog={2};Uid={3};Pwd={4}", Params);
				}
				else
				{
					query = string.Format("Provider=SQLOLEDB.1;Data Source={0};Initial Catalog={1};Integrated Security=SSPI;", Import.MU_SERVER, Import.MU_DB);
				}

				Import.Mu_Connection.ConnectionString = query;

				Import.Mu_Connection.Open();

				if (Import.USE_ME == 1)
				{
					Import.Me_Connection = new OleDbConnection();

					if (Import.ME_TRUSTED != 1)
					{
						object[] Params = new object[] { Import.ME_SERVER, Import.ME_PORT, Import.ME_DB, Import.ME_DB_USER, Import.ME_DB_PASS };

						query = string.Format("Provider=SQLOLEDB.1;Data Source={0},{1};Initial Catalog={2};Uid={3};Pwd={4}", Params);
					}
					else
					{
						query = string.Format("Provider=SQLOLEDB.1;Data Source={0};Initial Catalog={1};Integrated Security=SSPI;", Import.ME_SERVER, Import.ME_DB);
					}

					Import.Me_Connection.ConnectionString = query;

					Import.Me_Connection.Open();
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);

				Environment.Exit(0);
			}
		}

		public static bool Mu_ExecuteSQL(string sql)
		{
			bool flag;

			try
			{
				OleDbCommand cmd = new OleDbCommand(sql, Import.Mu_Connection);

				cmd.ExecuteNonQuery();

				cmd.Dispose();

				flag = true;
			}
			catch (Exception exception1)
			{
				Exception exception = exception1;

				string[] strArrays = new string[] { "SQL：", sql, "\nError:", exception.Message, "\nSource:", exception.Source, "\nTrace:", exception.StackTrace };

				MessageBox.Show(string.Concat(strArrays), "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

				return false;
			}

			return flag;
		}

		public static bool Me_ExecuteSQL(string sql)
		{
			bool flag;

			try
			{
				OleDbCommand cmd = new OleDbCommand(sql, Import.Me_Connection);

				cmd.ExecuteNonQuery();

				cmd.Dispose();

				flag = true;
			}
			catch (Exception exception1)
			{
				Exception exception = exception1;

				string[] strArrays = new string[] { "SQL：", sql, "\nError:", exception.Message, "\nSource:", exception.Source, "\nTrace:", exception.StackTrace };

				MessageBox.Show(string.Concat(strArrays), "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

				return false;
			}

			return flag;
		}

		public static void ChangeDB()
		{
			Import.Mu_Connection.Close();

			Import.Mu_Connection = new OleDbConnection();

			string query;

			try
			{
				if (Import.MU_TRUSTED != 1)
				{
					object[] Params = new object[] { Import.ME_SERVER, Import.ME_PORT, Import.ME_DB, Import.ME_DB_USER, Import.ME_DB_PASS };

					query = string.Format("Provider=SQLOLEDB.1;Data Source={0},{1};Initial Catalog={2};Uid={3};Pwd={4}", Params);
				}
				else
				{
					query = string.Format("Provider=SQLOLEDB.1;Data Source={0};Initial Catalog={1};Integrated Security=SSPI;", Import.MU_SERVER, Import.MU_DB);
				}

				Import.Mu_Connection.ConnectionString = query;

				Import.Mu_Connection.Open();
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);

				Environment.Exit(0);
			}
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