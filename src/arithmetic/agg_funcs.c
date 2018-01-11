#include <float.h>
#include "agg_funcs.h"
#include "aggregate.h"
#include "repository.h"
#include "../value.h"
#include <math.h>
#include "../util/qsort.h"

typedef struct {
    size_t num;
    double total;
} __agg_sumCtx;

int __agg_sumStep(AggCtx *ctx, SIValue *argv, int argc) {
    // convert the value of the input sequence to a double if possible
    __agg_sumCtx *ac = Agg_FuncCtx(ctx);

    double n;
    for(int i = 0; i < argc; i ++) {
        if (!SIValue_ToDouble(&argv[i], &n)) {
            if (!SIValue_IsNullPtr(&argv[i])) {
                // not convertible to double!
                return Agg_SetError(ctx,
                                    "SUM Could not convert upstream value to double");
            } else {
                return AGG_OK;
            }
        }

        ac->num++;
        ac->total += n;
    }

    return AGG_OK;
}

int __agg_sumReduceNext(AggCtx *ctx) {
    __agg_sumCtx *ac = Agg_FuncCtx(ctx);
    Agg_SetResult(ctx, SI_DoubleVal(ac->total));
    return AGG_OK;
}

AggCtx* Agg_SumFunc() {
    __agg_sumCtx *ac = malloc(sizeof(__agg_sumCtx));
    ac->num = 0;
    ac->total = 0;
    
    return Agg_Reduce(ac, __agg_sumStep, __agg_sumReduceNext);
}

//------------------------------------------------------------------------

typedef struct {
    size_t count;
    double total;
} __agg_avgCtx;

int __agg_avgStep(AggCtx *ctx, SIValue *argv, int argc) {
    // convert the value of the input sequence to a double if possible
    __agg_avgCtx *ac = Agg_FuncCtx(ctx);

    double n;
    for(int i = 0; i < argc; i ++) {
        if (!SIValue_ToDouble(&argv[i], &n)) {
            if (!SIValue_IsNullPtr(&argv[i])) {
                // not convertible to double!
                return Agg_SetError(ctx,
                                    "AVG Could not convert upstream value to double");
            } else {
                return AGG_OK;
            }
        }


        ac->count++;
        ac->total += n;
    }

    return AGG_OK;
}

int __agg_avgReduceNext(AggCtx *ctx) {
    __agg_avgCtx *ac = Agg_FuncCtx(ctx);

    if(ac->count > 0) {
        Agg_SetResult(ctx, SI_DoubleVal(ac->total / ac->count));
    } else {
        Agg_SetResult(ctx, SI_DoubleVal(0));
    }
    return AGG_OK;
}

AggCtx* Agg_AvgFunc() {
    __agg_avgCtx *ac = malloc(sizeof(__agg_avgCtx));
    ac->count = 0;
    ac->total = 0;
    
    return Agg_Reduce(ac, __agg_avgStep, __agg_avgReduceNext);
}

//------------------------------------------------------------------------

typedef struct {
    double max;
} __agg_maxCtx;

int __agg_maxStep(AggCtx *ctx, SIValue *argv, int argc) {
    // convert the value of the input sequence to a double if possible
    __agg_maxCtx *ac = Agg_FuncCtx(ctx);

    double n;
    for(int i = 0; i < argc; i ++) {
        if (!SIValue_ToDouble(&argv[i], &n)) {
            if (!SIValue_IsNullPtr(&argv[i])) {
                // not convertible to double!
                return Agg_SetError(ctx,
                                    "MAX Could not convert upstream value to double");
            } else {
                return AGG_OK;
            }
        }

        if(n > ac->max) {
            ac->max = n;
        }
    }

    return AGG_OK;
}

int __agg_maxReduceNext(AggCtx *ctx) {
    __agg_maxCtx *ac = Agg_FuncCtx(ctx);
    Agg_SetResult(ctx, SI_DoubleVal(ac->max));
    return AGG_OK;
}

AggCtx* Agg_MaxFunc() {
    __agg_maxCtx *ac = malloc(sizeof(__agg_maxCtx));
    ac->max = -DBL_MAX;
    
    return Agg_Reduce(ac, __agg_maxStep, __agg_maxReduceNext);
}

//------------------------------------------------------------------------

typedef struct {
    double min;
} __agg_minCtx;

int __agg_minStep(AggCtx *ctx, SIValue *argv, int argc) {
    // convert the value of the input sequence to a double if possible
    __agg_minCtx *ac = Agg_FuncCtx(ctx);

    double n;
    for(int i = 0; i < argc; i ++) {
        if (!SIValue_ToDouble(&argv[i], &n)) {
            if (!SIValue_IsNullPtr(&argv[i])) {
                // not convertible to double!
                return Agg_SetError(ctx,
                                    "MIN Could not convert upstream value to double");
            } else {
                return AGG_OK;
            }
        }

        if(n < ac->min) {
            ac->min = n;
        }
    }

    return AGG_OK;
}

int __agg_minReduceNext(AggCtx *ctx) {
    __agg_minCtx *ac = Agg_FuncCtx(ctx);
    Agg_SetResult(ctx, SI_DoubleVal(ac->min));
    return AGG_OK;
}

AggCtx* Agg_MinFunc() {
    __agg_minCtx *ac = malloc(sizeof(__agg_minCtx));
    ac->min = DBL_MAX;
    
    return Agg_Reduce(ac, __agg_minStep, __agg_minReduceNext);
}

//------------------------------------------------------------------------

typedef struct {
    size_t count;
} __agg_countCtx;

int __agg_countStep(AggCtx *ctx, SIValue *argv, int argc) {
    __agg_countCtx *ac = Agg_FuncCtx(ctx);

    for(int i = 0; i < argc; i ++) {
        ac->count++;
    }
    return AGG_OK;
}

int __agg_countReduceNext(AggCtx *ctx) {
    __agg_countCtx *ac = Agg_FuncCtx(ctx);
    Agg_SetResult(ctx, SI_DoubleVal(ac->count));
    return AGG_OK;
}

AggCtx* Agg_CountFunc() {
    __agg_countCtx *ac = malloc(sizeof(__agg_countCtx));
    ac->count = 0;
    
    return Agg_Reduce(ac, __agg_countStep, __agg_countReduceNext);
}

//------------------------------------------------------------------------

typedef struct {
    double percentile;
    double *values;
    size_t count;
} __agg_percCtx;

// This function is agnostic as to percentile method
int __agg_percStep(AggCtx *ctx, SIValue *argv, int argc) {

    __agg_percCtx *ac = Agg_FuncCtx(ctx);

    // The first argument is the percentile value. This only actually needs to be set once
    // (and is not actually used except in Reduce)
    if (!SIValue_ToDouble(&argv[0], &ac->percentile)) {
        return Agg_SetError(ctx,
                "PERC_DISC Could not convert percentile argument to double");
    }
    if (ac->percentile < 0 || ac->percentile > 1) {
        return Agg_SetError(ctx,
                "PERC_DISC requires a percentile value between 0.0 and 1.0");
    }

    double n;
    for (int i = 1; i < argc; i ++) {
        if (!SIValue_ToDouble(&argv[i], &n)) {
            if (!SIValue_IsNullPtr(&argv[i])) {
                // not convertible to double!
                return Agg_SetError(ctx,
                        "PERC_DISC Could not convert upstream value to double");
            } else {
                return AGG_OK;
            }
        }
        ac->values[ac->count] = n;
        ac->count ++;
    }

    return AGG_OK;
}

int __agg_percDiscReduceNext(AggCtx *ctx) {
    __agg_percCtx *ac = Agg_FuncCtx(ctx);

    #define ISLT(a,b) ((*a) < (*b))
    QSORT(double, ac->values, ac->count, ISLT);
    // If ac->percentile == 0, employing this formula would give an index of -1
    int idx = ac->percentile > 0 ? ceil(ac->percentile * ac->count) - 1 : 0;
    if (idx >= ac->count) {
        return Agg_SetError(ctx,
                "PERC_DISC tried to read an out-of-range value");
    }
    double n = ac->values[idx];
    Agg_SetResult(ctx, SI_DoubleVal(n));

    return AGG_OK;
}

AggCtx* Agg_PercDiscFunc() {
    __agg_percCtx *ac = malloc(sizeof(__agg_percCtx));
    ac->count = 0;
    // TODO TODO We should have access to the exact number of values we will be iterating over
    ac->values = malloc(10000 * sizeof(double));
    return Agg_Reduce(ac, __agg_percStep, __agg_percDiscReduceNext);
}

//------------------------------------------------------------------------

void Agg_RegisterFuncs() {
    Agg_RegisterFunc("sum", Agg_SumFunc);
    Agg_RegisterFunc("avg", Agg_AvgFunc);
    Agg_RegisterFunc("max", Agg_MaxFunc);
    Agg_RegisterFunc("min", Agg_MinFunc);
    Agg_RegisterFunc("count", Agg_CountFunc);
    Agg_RegisterFunc("perc_d", Agg_PercDiscFunc);
}