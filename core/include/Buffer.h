#ifndef BUFFER_H_
#define BUFFER_H_

#include <ext/rope>
#include <vector>
#include <map>
#include "Cursor.h"
#include "LineBoundaries.h"

using namespace __gnu_cxx;
using namespace std;


class Buffer {
private:
    crope *rope;
    Cursor cursor;

    LineBoundaries* line_boundaries;

    bool is_whitespace(char c);

public:
    Buffer();
    explicit Buffer(const char* filename);
    ~Buffer();

    // TODO: Better error handeling
    class file_char_prod : public char_producer<char> {
    public:
        FILE* f;

        explicit file_char_prod(const char* file_name) {
            if (nullptr == (f = fopen(file_name, "rb")))
                exit(13);
        }

        ~file_char_prod() override { fclose(f); }

        void operator()(size_t start_pos, size_t len, char* buffer) override {
            if (fseek(f, start_pos, SEEK_SET)) exit(13);
            if (fread(buffer, sizeof(char), len, f) < len) exit(13);
        }

        size_t len() {
            if (fseek(f, 0, SEEK_END)) exit(13);
            return static_cast<size_t>(ftell(f));
        }
    };

    struct Line { crope rope; size_t start_idx; };

    size_t get_x();
    size_t get_idx();
    size_t get_y();
    const crope& get_rope();
    crope::const_iterator begin();
    crope::const_iterator end();
    size_t length();
    bool backspace();
    bool insert(char ch);
    bool insert(string s);
    void forward(size_t n = 1);
    void back(size_t n = 1);
    void up();
    void down();
    void beginning_of_line();
    void end_of_line();
    void back_word();
    void forward_word();
    void beginning_of_buffer();
    void end_of_buffer();

    vector<Line> get_lines(size_t start, size_t num);
    Line get_line(size_t y);

    const char* filename;
};

#endif
