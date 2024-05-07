/*
    Fill in this block comment before submitting.
    Name 1: Hai Ning Liu
    Name 2: None
	I work alone
    If you worked alone, indicate that as well.    
*/


#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string addBigInt(string s1, string s2);
string subBigInt(string s1, string s2);
string addzeros(string s1, int numOfZeros);
string mult3aHelper(string s1, string s2, int digits);

string mult4(string s1, string s2)
{	
	int numOfZeros = 0;
	string result = "0";
	for (int i = s2.length() - 1; i >= 0; i--) {

		int innerNumOfZeros = 0;
		string innerResult = "0";
		for(int j = s1.length() - 1; j >= 0; j--) {
			// convert char to a string
			std::string digStr1(1, s1[j]);
			int num1 = std::stoi(digStr1);

			std::string digStr2(1, s2[i]);
			int num2 = std::stoi(digStr2);

			std::string s = std::to_string(num1 * num2);
			innerResult = addBigInt(innerResult, addzeros(s, innerNumOfZeros));
			
			innerNumOfZeros++;
		}
		result = addBigInt(result, addzeros(innerResult, numOfZeros));
		numOfZeros++;
	}
	// Your code here!
	return result;
}

string addzeros(string s1, int numOfZeros) {
	for (int i = 0; i < numOfZeros; i++) {
		s1.append("0");
	}
	return s1;
}

string mult3a(string s1, string s2)
{
	int digits= s1.length();
	return mult3aHelper(s1, s2, digits);
}

string mult3aHelper(string s1, string s2, int digits) {
	if (digits == 1) {
		return std::to_string(std::stoi(s1) * std::stoi(s2));
	}

	// split the string from the right!!, spliting from the left won't work for odd digits integer
	string s1MS = s1.substr(0, s1.length() - digits/2);
	string s1LS = s1.substr(s1.length() - digits/2, digits/2);
	string s2MS = s2.substr(0, s2.length() - digits/2);
	string s2LS = s2.substr(s2.length() - digits/2, digits/2);

	string part1A = mult3aHelper(s1MS, s2MS, digits/2);
	string part1 = addzeros(part1A, digits);

	string part3 = mult3aHelper(s1LS, s2LS, digits/2);

	// addBigInt(...) might result in odd number of digits!, remember to split the string from the right
	string part2A = mult3aHelper(addBigInt(s1MS, s1LS), addBigInt(s2MS, s2LS), digits/2);
	string part2B = subBigInt(part3, subBigInt(part1A, part2A));
	string part2 = addzeros(part2B, digits/2);

	return addBigInt(addBigInt(part1, part2), part3);
}


string mult3b(string s1, string s2)
{
	// Your code here!
	int digits= s1.length();
	if (digits > 8) {
		return mult3aHelper(s1, s2, digits);
	} else {
		unsigned long long int op1 = std::stoi(s1);
		unsigned long long int op2 = std::stoi(s2);
		unsigned long long int result = op1 * op2;
		return std::to_string(result);
	}
}

// Integer string addition provided for your convenience
// I used this function in my own solution. This does not *guarantee* it will work in yours. 
// It depends on your own specific implementation of Karatsuba's/Schoolbook 
// Feel free to modify, update, whatever.
string addBigInt(string s1, string s2)
{
	if (s1.length() > s2.length())
		swap(s1, s2);

	string ssum = "";
	int n1 = s1.length(), n2 = s2.length();
	int diff = n2 - n1, carry = 0;

	for (int i = n1 - 1; i >= 0; i--) {
		int sum = ((s1[i] - '0') + (s2[i + diff] - '0') + carry);
		ssum.push_back(sum % 10 + '0');
		carry = sum / 10;
	}
	for (int i = n2 - n1 - 1; i >= 0; i--) {
		int sum = ((s2[i] - '0') + carry);
		ssum.push_back(sum % 10 + '0');
		carry = sum / 10;
	}
	if (carry)
		ssum.push_back(carry + '0');
	std::reverse(ssum.begin(), ssum.end());
	return ssum;
}

// Helper function used by subBigInt() below
// Return true if str1 is smaller than str2
bool isSmaller(string str1, string str2)
{
	int n1 = str1.length(), n2 = str2.length();

	if (n1 < n2) 
		return true;
	if (n2 > n1) 
		return false;

	for (int i = 0; i<n1; i++) {
		if (str1[i] < str2[i]) 
			return true;
		else if (str1[i] > str2[i])	
			return false;
	}
	return false;
}

// Integer string subtraction provided for your convenience
// I used this function in my own solution. This does not *guarantee* it will work in yours. 
// It depends on your own specific implementation of Karatsuba's/Schoolbook 
// Feel free to modify, update, whatever.
string subBigInt(string s1, string s2) 
{
	if (isSmaller(s1, s2)) 
		swap(s1, s2);
	
	string sdif = "";
	int n1 = s1.length(), n2 = s2.length();
	int diff = n1 - n2, carry = 0;

	for (int i = n2 - 1; i >= 0; i--) {
		int sub = ((s1[i + diff] - '0') - (s2[i] - '0') - carry);
		if (sub < 0) {
			sub = sub + 10;
			carry = 1;
		}
		else
			carry = 0;
		sdif.push_back(sub + '0');
	}
	for (int i = n1 - n2 - 1; i >= 0; i--) {
		if (s1[i] == '0' && carry) {
			sdif.push_back('9');
			continue;
		}
		int sub = ((s1[i] - '0') - carry);
		if (i > 0 || sub > 0)
			sdif.push_back(sub + '0');
		carry = 0;
	}
	std::reverse(sdif.begin(), sdif.end());

	return sdif;
}

 // int main() {
 // string result = mult3b ("12453234", "45657435");
 // cout << result << endl;
 // }
