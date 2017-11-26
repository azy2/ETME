#ifndef ETME_OPTIONAL_H
#define ETME_OPTIONAL_H

template <typename T>
class Optional {
public:
    explicit Optional(T v) : _val(v), _has_val(true) {
    }
    Optional() : _val(), _has_val(false) {}

    constexpr explicit operator bool() const noexcept {
        return _has_val;
    }
    constexpr const T& val() const & {
        return _val;
    }
private:
    const T _val;
    const bool _has_val;
};

#endif //ETME_OPTIONAL_H
