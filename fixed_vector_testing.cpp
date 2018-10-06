#include "gtest/gtest.h"
#include "fixed_vector.h"

namespace {

    void init_thous(fixed_vector<int, 1000>&v) {
        for (size_t i = 0; i < 1000; i++) {
            v.push_back(i);
            ASSERT_EQ(i, v.back());
        }
    }

    void check_copying(fixed_vector<int, 1000>&l, fixed_vector<int, 1000>&r) {
        for (std::size_t i = 0; i < 100; i++) {
            ASSERT_EQ(l[i], r[i]);
        }

        // reference/ptr check
        l[0] = 16;
        ASSERT_EQ(r[0], 0);
    }
}

TEST(basic, push_popback) {
    fixed_vector<int, 1000> v;
    init_thous(v);
    ASSERT_EQ(1000, v.size());

    for (size_t i = 1000; i > 0; i--) {
        ASSERT_EQ(i - 1, v.back());
        v.pop_back();
    }

    ASSERT_EQ(0, v.size());
}

TEST(basic, init_copy) {
    fixed_vector<int, 1000> v;

    init_thous(v);

    fixed_vector<int, 1000> vc(v);

    check_copying(v, vc);
}

TEST(basic, operator_assign) {
    fixed_vector<int, 1000> v;
    init_thous(v);
    fixed_vector<int, 1000> vc;
    vc = v;

    check_copying(v, vc);
}

TEST(iterators, range_based_for) {
    fixed_vector<int, 1000> v;
    init_thous(v);

    int cur = 0;
    for (auto it : v) {
        ASSERT_EQ(it, cur);
        cur++;
    }
}

TEST(iterators, range_based_const) {
    fixed_vector<int, 1000> v;
    init_thous(v);

    int cur = 0;
    for (const auto &it : v) {
        ASSERT_EQ(it, cur);
        cur++;
    }
}

TEST(iterators, insert_erase) {
    fixed_vector<int, 1000> v;
    for (int i = 0; i < 500; i++) {
        v.push_back(i);
    }

    v.insert(v.begin(), 8);
    ASSERT_EQ(501, v.size());
    ASSERT_EQ(8, v[0]);

    v.insert(v.begin() + 250, 808);
    ASSERT_EQ(808, v[250]);

    v.erase(v.begin());
    ASSERT_EQ(0, v.front());

    v.erase(v.end() - 1);
    ASSERT_EQ(498, v.back());

}

TEST(iterators, range_erase) {
    fixed_vector<int, 1000> v;
    init_thous(v);
    v.erase(v.begin(), v.end());
    ASSERT_EQ(v.size(), 0);
}