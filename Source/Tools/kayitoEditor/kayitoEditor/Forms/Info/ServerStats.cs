using kayito_Editor.Source;
using System;
using System.Data;
using System.Data.OleDb;
using System.Windows.Forms;

namespace kayito_Editor.Forms
{

    public partial class ServerStats : Form
    {
        public ServerStats()
        {
            InitializeComponent();
        }

        private void ServerStats_Load(object sender, EventArgs e)
        {
            if (((Import.USE_ME != 1) ? Import.Mu_Connection.State : Import.Me_Connection.State) != ConnectionState.Open)
            {
                MessageBox.Show("No connection.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                this.Dispose();
            }

            string query = null;

            try
            {
                this.Cursor = Cursors.WaitCursor;

                string[] Query = new string[] { "SELECT TOP 1 (SELECT COUNT(1) FROM [MEMB_INFO]) AS [a], (SELECT COUNT(1) FROM [MEMB_STAT] WHERE ([ConnectStat] = 1)) AS [b] FROM [dbo].[MEMB_INFO]" };

                query = string.Concat(Query);

                OleDbDataReader oleDbDataReader = (new OleDbCommand(query, ((Import.USE_ME != 1) ? Import.Mu_Connection : Import.Me_Connection))).ExecuteReader();

                if (oleDbDataReader.Read())
                {
                    this.Accounts_Cant.Text = string.Format("{0}",oleDbDataReader.GetValue(0));

                    this.Onlines_Cant.Text = string.Format("{0}", oleDbDataReader.GetValue(1));
                }

                query = "SELECT COUNT(*) from Character";

                int TotalChar = 0;

                oleDbDataReader = new OleDbCommand(query, Import.Mu_Connection).ExecuteReader();

                if (oleDbDataReader.Read())
                {
                    TotalChar = oleDbDataReader.GetInt32(0);
                    this.Characters_Cant.Text = string.Format("{0}", TotalChar);
                }

                query = "SELECT Class, COUNT(Class) as Cant from Character group by Class order by Class asc";

                oleDbDataReader = new OleDbCommand(query, Import.Mu_Connection).ExecuteReader();

                while (oleDbDataReader.Read())
                {
                    int Class = oleDbDataReader.GetByte(0);

                    ListViewItem lista = new ListViewItem(Import.Classes.ContainsKey(Class) ? Import.Classes[Class] : Import.Classes[-1]);

                    int ClassCant = oleDbDataReader.GetInt32(1);

                    lista.SubItems.Add(string.Format("{0:00}% ({1})", (ClassCant*100/TotalChar) , ClassCant));

                    this.Characters_List.Items.Add(lista);
                }

                oleDbDataReader.Close();
            }
            catch (Exception exception)
            {
                MessageBox.Show("SQL：" + query + "\nError:" + exception.Message + "\nSource:" + exception.Source + "\nTrace:" + exception.StackTrace, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

            this.Cursor = Cursors.Default;
        }
    }
}
