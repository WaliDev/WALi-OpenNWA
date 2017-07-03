#define DECLARE_LOG(BASE) \
    int icra_log ## BASE (int n) { \
        int i; \
        int r = 0; \
        __VERIFIER_assume(n > 0); \
        for(i = 1; i != n; i *= BASE) {  \
        /* for(i = 1; i < n; i *= BASE) { */ \
            r ++; \
        } \
        return r; \
    }

