#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <sstream>

using namespace std;

class StringHelper
{
public:

    /*
    static vector< char > string_convert_to_bytes( const char *str )
    {
        wstring_convert<std::codecvt_utf8<char>, char> converter;
        string string = converter.to_bytes( str );
        return vector< char >( string.begin(), string.end() );
    }
*/
	static string toLower(string source)
	{
		transform(source.begin(), source.end(), source.begin(), ::tolower);
		return source;
	}

	static string toUpper(string source)
	{
		transform(source.begin(), source.end(), source.begin(), ::toupper);
		return source;
	}

	static string trimStart(string source, const string &trimChars = " \t\n\r\v\f")
	{
		return source.erase(0, source.find_first_not_of(trimChars));
	}

	static string trimEnd(string source, const string &trimChars = " \t\n\r\v\f")
	{
		return source.erase(source.find_last_not_of(trimChars) + 1);
	}

	static string trim(string source, const string &trimChars = " \t\n\r\v\f")
	{
		return trimStart(trimEnd(source, trimChars), trimChars);
	}

	static string replace(string source, const string &find, const string &replace)
	{
		size_t pos = 0;
		while ((pos = source.find(find, pos)) != string::npos)
		{
			source.replace(pos, find.length(), replace);
			pos += replace.length();
		}
		return source;
	}

	static bool startsWith(const string &source, const string &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(0, value.length(), value) == 0;
	}

	static bool endsWith(const string &source, const string &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(source.length() - value.length(), value.length(), value) == 0;
	}
/*
	static vector<string> split(const string &source, char delimiter)
	{
		vector<string> output;
		wistringstream ss(source);
		string nextItem;

		while (getline(ss, nextItem, delimiter))
		{
			output.push_back(nextItem);
		}

		return output;
	}
*/
	template<typename T>
	static string toString(const T &subject)
	{
		ostringstream ss;
		ss << subject;
		return ss.str();
	}

	/*
	template<typename T>
	static T fromString(const string &subject)
	{
		wistringstream ss(subject);
		T target;
		ss >> target;
		return target;
	}
*/

	static bool isEmptyOrWhiteSpace(const string &source)
	{
		if (source.length() == 0)
			return true;
		else
		{
			for (int index = 0; index < source.length(); index++)
			{
				if (!isspace(source[index]))
					return false;
			}

			return true;
		}
	}

	/*
	template<typename T>
	static string formatSimple(const string &input, T arg1)
	{
		wostringstream ss;
		int lastFormatChar = -1;
		int percent = -1;
		while ((percent = input.find(L'%', percent + 1)) > -1)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				int formatEnd = -1;
				string index;
				for (int i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == 's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == '$' && i + 1 < input.length() && input[i + 1] == 's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!isdigit(input[i]))
						break;
				}

				if (formatEnd > -1)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;

					if (index == L"1")
						ss << arg1;
					else
						throw runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}
*/

	/*
	template<typename T1, typename T2>
	static string formatSimple(const string &input, T1 arg1, T2 arg2)
	{
		wostringstream ss;
		int lastFormatChar = -1;
		int percent = -1;
		while ((percent = input.find(L'%', percent + 1)) > -1)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				int formatEnd = -1;
				string index;
				for (int i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == 's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == '$' && i + 1 < input.length() && input[i + 1] == 's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!isdigit(input[i]))
						break;
				}

				if (formatEnd > -1)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;

					if (index == L"1")
						ss << arg1;
					else if (index == L"2")
						ss << arg2;
					else
						throw runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}
*/

	/*
	template<typename T1, typename T2, typename T3>
	static string formatSimple(const string &input, T1 arg1, T2 arg2, T3 arg3)
	{
		wostringstream ss;
		int lastFormatChar = -1;
		int percent = -1;
		while ((percent = input.find(L'%', percent + 1)) > -1)
		{
			if (percent + 1 < input.length())
			{
				if (input[percent + 1] == L'%')
				{
					percent++;
					continue;
				}

				int formatEnd = -1;
				string index;
				for (int i = percent + 1; i < input.length(); i++)
				{
					if (input[i] == 's')
					{
						index = L"1";
						formatEnd = i;
						break;
					}
					else if (input[i] == '$' && i + 1 < input.length() && input[i + 1] == 's')
					{
						index = input.substr(percent + 1, i - percent - 1);
						formatEnd = i + 1;
						break;
					}
					else if (!isdigit(input[i]))
						break;
				}

				if (formatEnd > -1)
				{
					ss << input.substr(lastFormatChar + 1, percent - lastFormatChar - 1);
					lastFormatChar = formatEnd;

					if (index == L"1")
						ss << arg1;
					else if (index == L"2")
						ss << arg2;
					else if (index == L"3")
						ss << arg3;
					else
						throw runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastFormatChar + 1 < input.length())
			ss << input.substr(lastFormatChar + 1);

		return ss.str();
	}
	 */
};
