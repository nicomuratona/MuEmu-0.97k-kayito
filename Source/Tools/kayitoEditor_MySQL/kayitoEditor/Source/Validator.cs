using System.Text.RegularExpressions;

namespace kayito_Editor.Source
{
	public class Validator
	{
		public Validator()
		{

		}

		public static bool Chinese(string str)
		{
			return Regex.IsMatch(str, "^[一-龥]+$");
		}

		public static bool Contact(string str)
		{
			return Regex.IsMatch(str, "(\\d{11})|(((\\d{3}-|\\d{4}-)?(\\d{8}|\\d{7})?))");
		}

		public static bool Date(string strDate)
		{
			return Regex.IsMatch(strDate, "^((((1[6-9]|[2-9]\\d)\\d{2})-(0?[13578]|1[02])-(0?[1-9]|[12]\\d|3[01]))|(((1[6-9]|[2-9]\\d)\\d{2})-(0?[13456789]|1[012])-(0?[1-9]|[12]\\d|30))|(((1[6-9]|[2-9]\\d)\\d{2})-0?2-(0?[1-9]|1\\d|2[0-8]))|(((1[6-9]|[2-9]\\d)(0[48]|[2468][048]|[13579][26])|((16|[2468][048]|[3579][26])00))-0?2-29-))$");
		}

		public static bool Email(string strEmail)
		{
			return Regex.IsMatch(strEmail, "^([\\w-\\.]+)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([\\w-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)$");
		}

		public static bool Empty(string str)
		{
			return Regex.Replace(str, "^\\s+|\\s+$", "") == "";
		}

		public static bool HasCHZN(string str)
		{
			return Regex.IsMatch(str, "[一-龥]");
		}

		public static bool HexString(string str)
		{
			return Regex.IsMatch(str, "^[A-Fa-f0-9]+$");
		}

		public static bool IP(string IP)
		{
			return Regex.IsMatch(IP, "^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
		}

		public static bool KeyWord(string str)
		{
			return Regex.IsMatch(str, "^[\u3000， ,A-Za-z0-9\\u4e00-\\u9fa5]+$");
		}

		public static bool Letter(string str)
		{
			return Regex.IsMatch(str, "^[A-Za-z]+$");
		}

		public static bool LetterOrDigit(string str)
		{
			return Regex.IsMatch(str, "^[A-Za-z0-9]+$");
		}

		public static bool Mobile(string mobile)
		{
			return Regex.IsMatch(mobile, "\\d{11}");
		}

		public static bool MyRegex(string str, string pattern)
		{
			return Regex.IsMatch(str, pattern);
		}

		public static bool NotEmpty(string str)
		{
			return Regex.Replace(str, "^\\s+|\\s+$", "") != "";
		}

		public static bool Number(string str)
		{
			return Regex.IsMatch(str, "^[0-9]+$");
		}

		public static bool NumberSign(string str)
		{
			return Regex.IsMatch(str, "^[+-]?[0-9]+$");
		}

		public static bool PassWord(string str)
		{
			return Regex.IsMatch(str, "(.|\\s){4,10}");
		}

		public static bool Phone(string strPhone)
		{
			return Regex.IsMatch(strPhone, "(\\d{3}-|\\d{4}-)?(\\d{8}|\\d{7})?");
		}

		public static bool Price(string strPrice)
		{
			return Regex.IsMatch(strPrice, "^(([0-9]+\\.[0-9]*[0-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
		}

		public static bool QQ(string strQQ)
		{
			return Regex.IsMatch(strQQ, "^[1-9]\\d{4,9}$");
		}

		public static bool Time(string strTime)
		{
			return Regex.IsMatch(strTime, "^((([0-1]?[0-9])|(2[0-3])):([0-5]?[0-9])(:[0-5]?[0-9])?)$");
		}

		public static bool URL(string strUrl)
		{
			return Regex.IsMatch(strUrl, "^(http|https)\\://([a-zA-Z0-9\\.\\-]+(\\:[a-zA-Z0-9\\.&%\\$\\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|localhost|([a-zA-Z0-9\\-]+\\.)*[a-zA-Z0-9\\-]+\\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{1,10}))(\\:[0-9]+)*(/($|[a-zA-Z0-9\\.\\,\\?\\'\\\\\\+&%\\$#\\=~_\\-]+))*$");
		}

		public static bool UserName(string str)
		{
			return Regex.IsMatch(str, "^[A-Za-z0-9_]{4,10}$");
		}

		public static bool Container(object var, string str)
		{
			return Regex.IsMatch(str, "^[A-Za-z0-9_]{4,10}$");
		}
	}
}