#pragma once

#include <ext/rope>
#include <string>

#include "caf/all.hpp"

using namespace __gnu_cxx;
using namespace caf;

using insert_atom = atom_constant<atom("insert")>;
using erase_atom = atom_constant<atom("erase")>;
using open_file_atom = atom_constant<atom("open_file")>;
using copy_atom = atom_constant<atom("copy")>;

CAF_ALLOW_UNSAFE_MESSAGE_TYPE(wrope::const_iterator);

using buffer_actor = typed_actor<reacts_to<insert_atom, size_t, wchar_t>,
                                 reacts_to<erase_atom, size_t, size_t>,
                                 replies_to<open_file_atom, std::string>::with<bool>,
                                 replies_to<copy_atom, size_t, size_t, wchar_t*>::with<bool>>;

struct buffer_state {
    wrope r;
};

buffer_actor::behavior_type buffer_actor_fun(buffer_actor::stateful_pointer<buffer_state> self);
