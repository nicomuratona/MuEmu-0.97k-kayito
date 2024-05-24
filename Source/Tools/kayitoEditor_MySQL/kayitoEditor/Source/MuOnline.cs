using kayito_Editor.Source;
using MySql.Data.MySqlClient;
using System;
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
				connection_string = $"SERVER={Import.MU_SERVER};PORT={Import.MU_PORT};DATABASE={Import.MU_DB};UID={Import.MU_DB_USER};PASSWORD={Import.MU_DB_PASS};";

				Import.Mu_Connection = new MySqlConnection(connection_string);

				Import.Mu_Connection.Open();

				if (Import.USE_ME == 1)
				{
					connection_string = $"SERVER={Import.ME_SERVER};PORT={Import.ME_PORT};DATABASE={Import.ME_DB};UID={Import.ME_DB_USER};PASSWORD={Import.ME_DB_PASS};";

					Import.Me_Connection = new MySqlConnection(connection_string);

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
				MySqlCommand cmd = new MySqlCommand(query, Import.Mu_Connection);

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
				MySqlCommand cmd = new MySqlCommand(query, Import.Me_Connection);

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