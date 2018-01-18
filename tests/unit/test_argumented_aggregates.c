#include <stdio.h>
#include <math.h>
#include "assert.h"
#include "../../src/arithmetic/arithmetic_expression.h"
#include "../../src/arithmetic/agg_funcs.h"

void _test_ar_func(AR_ExpNode *root, SIValue expected) {
    SIValue res = AR_EXP_Evaluate(root);

    if(res.doubleval != expected.doubleval) {
        printf("res.doubleval: %lf expected.doubleval: %lf\n", res.doubleval, expected.doubleval);
    }

    assert(res.doubleval == expected.doubleval);
}

void test_percentile_cont() {
    // Percentiles to check
    AR_ExpNode *zero = AR_EXP_NewConstOperandNode(SI_DoubleVal(0));
    AR_ExpNode *dot_one = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.1));
    AR_ExpNode *one_third = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.33));
    AR_ExpNode *half = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.5));
    AR_ExpNode *one = AR_EXP_NewConstOperandNode(SI_DoubleVal(1));

    AR_ExpNode *test_percentiles[5] = {zero, dot_one, one_third, half, one};
    /*
     * The correct value for each percentile will be:
     * x = percentile * (count - 1)
     * if x is an integer return value[x]
     * else:
     * (value[floor(x)] * fraction(x)) + (value[ceil(x)] * (1 - fraction(x)))
     */
    double expected_values[5];
    double arr[5] = {2, 4, 6, 8, 10};
    int count = 5;

    double percentile_doubles[5] = {0, 0.1, 0.33, 0.5, 1};
    double x, int_val, fraction_val;
    int lower_idx, upper_idx;
    double lower, upper;

    for (int i = 0; i < 5; i ++) {
        x = percentile_doubles[i] * (count - 1);
        lower_idx =  floor(x);
        upper_idx = ceil(x);

        if (lower_idx == upper_idx || lower_idx == (count - 1)) {
            expected_values[i] = arr[lower_idx];
            continue;
        }

        lower = arr[lower_idx];
        upper = arr[upper_idx];

        expected_values[i] = (lower * ((double)upper_idx - x)) + (upper * (x - (double)lower_idx));
    }

    AR_ExpNode *perc;
    SIValue result;
    for (int i = 0; i < 5; i ++) {
        perc = AR_EXP_NewOpNode("percentileCont", 6);
        for (int j = 1; j <= 5; j ++) {
            perc->op.children[j - 1] = AR_EXP_NewConstOperandNode(SI_DoubleVal(j * 2));
        }
        // The last child of this node will be the requested percentile
        perc->op.children[5] = test_percentiles[i];
        AR_EXP_Aggregate(perc);
        // Reduce sorts the list and applies the percentile formula
        AR_EXP_Reduce(perc);
        result = AR_EXP_Evaluate(perc);
        assert(expected_values[i] == result.doubleval);
        AR_EXP_Free(perc);
    }

    printf("test_percentileCont - PASS!\n");
}
void test_percentile_disc() {
    // Percentiles to check
    AR_ExpNode *zero = AR_EXP_NewConstOperandNode(SI_DoubleVal(0));
    AR_ExpNode *dot_one = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.1));
    AR_ExpNode *one_third = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.33));
    AR_ExpNode *half = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.5));
    AR_ExpNode *one = AR_EXP_NewConstOperandNode(SI_DoubleVal(1));

    AR_ExpNode *test_percentiles[5] = {zero, dot_one, one_third, half, one};
    // percentileDisc should always return a value actually contained in the set
    int expected[5] = {0, 0, 1, 2, 4};

    AR_ExpNode *perc;
    for (int i = 0; i < 5; i ++) {
        perc = AR_EXP_NewOpNode("percentileDisc", 6);
        for (int j = 1; j <= 5; j ++) {
            perc->op.children[j - 1] = AR_EXP_NewConstOperandNode(SI_DoubleVal(j * 2));
        }
        // The last child of this node will be the requested percentile
        perc->op.children[5] = test_percentiles[i];
        AR_EXP_Aggregate(perc);
        // Reduce sorts the list and applies the percentile formula
        AR_EXP_Reduce(perc);
        _test_ar_func(perc, AR_EXP_Evaluate(perc->op.children[expected[i]]));
        AR_EXP_Free(perc);
    }

    printf("test_percentileDisc - PASS!\n");
}

int main(int argc, char **argv) {
    AR_RegisterFuncs();
    Agg_RegisterFuncs();

    test_percentile_disc();
    test_percentile_cont();

    return 0;
}