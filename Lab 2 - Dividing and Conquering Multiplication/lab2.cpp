#include <iostream>
#include <string>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "mult.hpp"

using namespace std;
using namespace std::chrono; // Namespace for fancy ms timing

// Return random integer string of length n
string randIntString(int n)
{
	string s(n, '0');
	s[0] = rand() % 9 + '1'; // Avoid leading 0
	for (int i = 1; i < n; i++)
		s[i] = rand() % 10 + '0';
	return s;
}

int main()
{
	char name[] = "<Your name(s) here!>";

	srand((int)time(NULL));
	printf("\nCCPS616 - Lab 2 - %s\n\n", name);

	/********************************************/
	/* Sanity check #1: Example from the slides */
	/********************************************/
	{
		cout << "Sanity check #1: Example from the slides" << endl;
		string op1 = "3563474256143563", op2 = "8976558458718976";
		string sol = "31987734976412811376690928351488";
		cout << "  " << op1 << " * " << op2 << " = " << sol << endl;
		

		/*******************************************/
		/* Call your multiplication functions here */
		/*******************************************/
		string prod_m4  = mult4(op1, op2);
		string prod_m3a = mult3a(op1, op2);
		string prod_m3b = mult3b(op1, op2);
		/*******************************************/
		/*******************************************/

		
		if (prod_m4 == sol) cout << "  mult4...  PASSED!" << endl;
		else cout << "  mult4...  FAILED: " << prod_m4 << endl;
		
		if (prod_m3a == sol) cout << "  mult3a... PASSED!" << endl;
		else cout << "  mult3a... FAILED: " << prod_m3a << endl;

		if (prod_m3b == sol) cout << "  mult3b... PASSED!" << endl;
		else cout << "  mult3b... FAILED: " << prod_m3b << endl;
	}

	/****************************************************************/
	/* Sanity check #2: Small strings tested against primitive mult */
	/****************************************************************/
	{
		cout << "\nSanity check #2: Small strings tested against primitive *" << endl;
        int m4Correct = 0;
        int m3aCorrect = 0;
        int m3bCorrect = 0;
		for (int i = 0; i < 10000; i++)
		{
			// Max Length of 8 means long long unsigned won't overflow
			string op1 = randIntString(8), op2 = randIntString(8);
			// Product using primitive multiplication
			string prod(to_string(stoull(op1) * stoull(op2)));


			/*******************************************/
			/* Call your multiplication functions here */
			/*******************************************/
			string prod_m4  = mult4(op1, op2);
			string prod_m3a = mult3a(op1, op2);
			string prod_m3b = mult3b(op1, op2);
			/*******************************************/
			/*******************************************/
			
			
			// Assert will fail if results don't match primitive mult
            //assert(prod == prod_m4 && prod == prod_m3a && prod == prod_m3b);
            if (prod == prod_m4) m4Correct++;
            if (prod == prod_m3a) m3aCorrect++;
            if (prod == prod_m3b) m3bCorrect++;
                       
		}
		printf("   m4  passed %5d/10000 (%.0lf%%)\n", m4Correct, 100*m4Correct/10000.0);
        printf("   m3a passed %5d/10000 (%.0lf%%)\n", m3aCorrect, 100*m3aCorrect/10000.0);
        printf("   m3b passed %5d/10000 (%.0lf%%)\n", m3bCorrect, 100*m3bCorrect/10000.0);
       
	}

	/******************************************************************/
	/* Time for the real tests! Time all three and verify correctness */
	/******************************************************************/
	{
		// Add string sizes as you see fit, but these are a good starting point
		int string_sizes[] = { 512, 1024, 2048, 4096 }; 
		int n2avg = 10; // Average over 10 trials once again
                
		for (int n : string_sizes)
		{
			int m4_time = 0, m3a_time = 0, m3b_time = 0;
			string op1 = randIntString(n);
			string op2 = randIntString(n);
			string prod_m4, prod_m3a, prod_m3b;
            
			printf("mult4()  %5d digits: ", n);
			for (int k = 0; k < n2avg; k++)
			{
				auto begin = high_resolution_clock::now();

				/********** Call mult4() here **********/
				prod_m4 = mult4(op1, op2);
				/***************************************/

				auto dur = high_resolution_clock::now() - begin;
				m4_time += (int)duration_cast<milliseconds>(dur).count();
				std::cout << "." << std::flush;
			}
			printf(" Avg: %dms\n", m4_time / n2avg);


			printf("mult3a() %5d digits: ", n);
			for (int k = 0; k < n2avg; k++)
			{
				auto begin = high_resolution_clock::now();

				/********** Call mult3a() here **********/
				prod_m3a = mult3a(op1, op2);
				/****************************************/

				auto dur = high_resolution_clock::now() - begin;
				m3a_time += (int)duration_cast<milliseconds>(dur).count();
				std::cout << "." << std::flush;
			}
			printf(" Avg: %dms\n", m3a_time / n2avg);


			printf("mult3b() %5d digits: ", n);
			for (int k = 0; k < n2avg; k++)
			{
				auto begin = high_resolution_clock::now();

				/********** Call mult3b() here **********/
				prod_m3b = mult3b(op1, op2);
				/****************************************/

				auto dur = high_resolution_clock::now() - begin;
				m3b_time += (int)duration_cast<milliseconds>(dur).count();
				std::cout << "." << std::flush;
			}
			printf(" Avg: %dms\n", m3b_time / n2avg);

			// Make sure all three products are equal. Of course they could all still be wrong...
			if (prod_m4 == prod_m3a && prod_m3a == prod_m3b)
                printf("         %5d digits: PASSED (m4==m3a==m3b)\n", n);
            else
                printf("         %5d digits: FAILED (m4,m3a,m3b results differ)\n", n);

		}
	}

	cout << "All done!" << endl;
}


