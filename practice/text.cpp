#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

bool matches_pattern(const string &word, const string &pattern) {
    // Extract core from %x% (e.g., x)
    string core = pattern.substr(1, pattern.length() - 2);
    return word.find(core) != string::npos;
}

vector<string> split_into_words(const string &text) {
    vector<string> words;
    string word;
    for (char c : text) {
        if (isalnum(c) || c == '-') {
            word += c;
        } else {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
    }
    if (!word.empty())
        words.push_back(word);
    return words;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0)
            cerr << "Usage: mpirun -n <procs> " << argv[0] << " <input_file> <%pattern%>\n";
        MPI_Finalize();
        return 1;
    }

    string full_text, pattern = argv[2];

    double start_time = MPI_Wtime();

    // Master reads file
    if (rank == 0) {
        ifstream file(argv[1]);
        if (!file) {
            cerr << "Cannot open input file.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        stringstream buffer;
        buffer << file.rdbuf();
        full_text = buffer.str();
    }

    // Broadcast total size
    int full_len = full_text.length();
    MPI_Bcast(&full_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast text to all processes
    char* full_buffer = new char[full_len + 1];
    if (rank == 0)
        strcpy(full_buffer, full_text.c_str());
    MPI_Bcast(full_buffer, full_len + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Broadcast pattern
    int pat_len = pattern.length();
    MPI_Bcast(&pat_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    char* pat_buffer = new char[pat_len + 1];
    if (rank == 0)
        strcpy(pat_buffer, pattern.c_str());
    MPI_Bcast(pat_buffer, pat_len + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    pattern = string(pat_buffer);

    // Divide text
    int chunk = full_len / size;
    int start = rank * chunk;
    int end = (rank == size - 1) ? full_len : start + chunk;

    string local_text(full_buffer + start, full_buffer + end);

    delete[] full_buffer;
    delete[] pat_buffer;

    // Extract words and match
    vector<string> words = split_into_words(local_text);
    int local_count = 0;
    for (const string &word : words) {
        if (matches_pattern(word, pattern))
            local_count++;
    }

    // Reduce
    int total_count;
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        cout << "Total matches for pattern \"" << pattern << "\": " << total_count << endl;
        cout << "Execution time: " << (end_time - start_time) << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
