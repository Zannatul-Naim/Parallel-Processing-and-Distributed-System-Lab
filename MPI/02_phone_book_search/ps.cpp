#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

void send_string(const string &text, int dest) {
    int len = text.size() + 1;
    MPI_Send(&len, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    MPI_Send(text.c_str(), len, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
}

string receive_string(int src) {
    int len;
    MPI_Recv(&len, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    char *buf = new char[len];
    MPI_Recv(buf, len, MPI_CHAR, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    string result(buf);
    delete[] buf;
    return result;
}

vector<string> split_words(const string &text) {
    vector<string> words;
    string word;
    for (char ch : text) {
        if (isalnum(ch))
            word += tolower(ch);
        else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) words.push_back(word);
    return words;
}

map<string, int> count_words(const vector<string> &words) {
    map<string, int> freq;
    for (const string &w : words) {
        freq[w]++;
    }
    return freq;
}

string serialize_map(const map<string, int> &freq) {
    string result;
    for (const auto &[word, count] : freq) {
        result += word + ":" + to_string(count) + "\n";
    }
    return result;
}

map<string, int> deserialize_map(const string &text) {
    map<string, int> freq;
    istringstream iss(text);
    string line;
    while (getline(iss, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string word = line.substr(0, pos);
            int count = stoi(line.substr(pos + 1));
            freq[word] += count;
        }
    }
    return freq;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) cerr << "Usage: mpirun -np <procs> ./wordcount <filename>\n";
        MPI_Finalize();
        return 1;
    }

    double start_time, end_time;
    string full_text;

    if (rank == 0) {
        ifstream infile(argv[1]);
        if (!infile) {
            cerr << "Cannot open file.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        stringstream ss;
        ss << infile.rdbuf();
        full_text = ss.str();
        infile.close();

        start_time = MPI_Wtime();

        int total_len = full_text.size();
        int chunk_size = (total_len + size - 1) / size;

        for (int i = 1; i < size; ++i) {
            int start = i * chunk_size;
            int end = min(start + chunk_size, total_len);
            if (start >= total_len) {
                send_string("", i);
                continue;
            }
            // Ensure no word is split mid-way
            while (end < total_len && isalnum(full_text[end])) end++;
            send_string(full_text.substr(start, end - start), i);
        }
        // Master also works
        full_text = full_text.substr(0, min(chunk_size, total_len));
    } else {
        full_text = receive_string(0);
    }

    vector<string> words = split_words(full_text);
    map<string, int> local_freq = count_words(words);

    if (rank != 0) {
        string serialized = serialize_map(local_freq);
        send_string(serialized, 0);
    } else {
        for (int i = 1; i < size; ++i) {
            string received = receive_string(i);
            map<string, int> temp_map = deserialize_map(received);
            for (const auto &[word, count] : temp_map)
                local_freq[word] += count;
        }

        vector<pair<string, int>> word_freq(local_freq.begin(), local_freq.end());
        sort(word_freq.begin(), word_freq.end(), [](auto &a, auto &b) {
            return b.second < a.second;
        });

        end_time = MPI_Wtime();

        cout << "Top 10 Most Frequent Words:\n";
        for (int i = 0; i < min(10, (int)word_freq.size()); ++i) {
            cout << word_freq[i].first << ": " << word_freq[i].second << endl;
        }
        printf("Total processing time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
