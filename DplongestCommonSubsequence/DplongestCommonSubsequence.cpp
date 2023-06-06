#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

std::string findLongestCommonSubsequence(const std::string &str1, const std::string &str2)
{
    int m = str1.length();
    int n = str2.length();
    if (m != n)
        return "";

    std::vector<int> dp(n + 1, 0);
    std::vector<int> prev_dp(n + 1, 0);

    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                dp[j] = prev_dp[j - 1] + 1;
            }
            else
            {
                dp[j] = std::max(dp[j - 1], prev_dp[j]);
            }
        }
        prev_dp = dp;
    }

    std::string longestSubsequence;
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        if (str1[i - 1] == str2[j - 1])
        {
            longestSubsequence = str1[i - 1] + longestSubsequence;
            i--;
            j--;
        }
        else if (dp[j - 1] > dp[j])
        {
            j--;
        }
        else
        {
            i--;
        }
    }

    return longestSubsequence;
}

void runTests()
{
    std::ofstream outputFile("results.txt");

    std::vector<int> sizes = {1000, 2000, 3000, 4000, 5000, 10000, 20000};

    for (int n : sizes)
    {
        std::string str1(n, 'A');
        std::string str2(n, 'B');

        auto start = std::chrono::high_resolution_clock::now();
        std::string longestSubsequence = findLongestCommonSubsequence(str1, str2);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        outputFile << n << " " << duration << std::endl;
    }

    outputFile.close();
}

int main()
{
    runTests();

    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set xlabel 'Length of sequence'\n");
        fprintf(gnuplotPipe, "set ylabel 'Execution time (ms)'\n");
        fprintf(gnuplotPipe, "set title 'Execution Time vs. Sequence Length'\n");
        fprintf(gnuplotPipe, "plot 'results.txt' with linespoints\n");
        fflush(gnuplotPipe);
        getchar();
        fprintf(gnuplotPipe, "exit\n");
        pclose(gnuplotPipe);
    }
}