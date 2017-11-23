#include "buffer.h"

class file_char_prod : public char_producer<wchar_t> {
public:
    FILE* f;
    file_char_prod(const char* file_name) {
        if (NULL == (f = fopen(file_name, "rb")))
            exit(1);
    }

    ~file_char_prod() { fclose(f); }

    virtual void operator()(size_t start_pos, size_t len, wchar_t* buffer) {
        if (fseek(f, start_pos, SEEK_SET)) exit(1);
        if (fread(buffer, sizeof(char), len, f) < len) exit(1);
    }

    size_t len() {
        if (fseek(f, 0, SEEK_END)) exit(1);
        return ftell(f);
    }
};

buffer_actor::behavior_type buffer_actor_fun(buffer_actor::stateful_pointer<buffer_state> self) {
    return {
        [=](insert_atom, size_t pos, wchar_t c) {
            self->state.r.insert(pos, c);
        },
            [=](erase_atom, size_t i, size_t n) {
                self->state.r.erase(i, n);
            },
                [=](open_file_atom, std::string filename) {
                    file_char_prod* fcp = new file_char_prod(filename.c_str());
                    self->state.r = rope<wchar_t>(fcp, fcp->len(), true);
                    return true;
                },

                    [=](copy_atom, size_t pos, size_t n, wchar_t* buf) {
                        self->state.r.copy(pos, n, buf);
                        return true;
                    }
    };
}
