#include <stdio.h>
#include "../../src/arithmetic/arithmetic_expression.h"
#include "assert.h"

void _test_ar_func(AR_ExpNode *root, SIValue expected) {
    SIValue res = AR_EXP_Evaluate(root);

    if(res.doubleval != expected.doubleval) {
        printf("res.doubleval: %lf expected.doubleval: %lf\n", res.doubleval, expected.doubleval);
    }

    assert(res.doubleval == expected.doubleval);
}

void test_percentile_disc() {
    // Percentiles to check
    AR_ExpNode *zero = AR_EXP_NewConstOperandNode(SI_DoubleVal(0));
    AR_ExpNode *dot_one = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.1));
    AR_ExpNode *half = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.5));
    AR_ExpNode *one = AR_EXP_NewConstOperandNode(SI_DoubleVal(1));
    AR_ExpNode *one_third = AR_EXP_NewConstOperandNode(SI_DoubleVal(0.33));

    AR_ExpNode *perc = AR_EXP_NewOpNode("PERC_D", 6);
    for (int i = 1; i <= 5; i ++) {
        perc->op.children[i] = AR_EXP_NewConstOperandNode(SI_DoubleVal(i * 2));
    }

    AR_ExpNode *test_percentiles[5] = {zero, dot_one, half, one, one_third};
    // percentile_disc should always return a value actually contained in the set
    AR_ExpNode *expected[5] = {perc->op.children[1], perc->op.children[1],
        perc->op.children[3], perc->op.children[5], perc->op.children[2]};

    for (int i = 0; i < 5; i ++) {
        // The first child of this node will be the requested percentile
        perc->op.children[0] = test_percentiles[i];
        // Aggregate sets the desired percentile value and adds each child to the list
        // TODO as such, every iteration of this loop will add all the values again,
        // which does not affect the calculation result (because percentiles) but still
        // makes this a kinda weird way to test
        AR_EXP_Aggregate(perc);
        // Reduce sorts the list and applies the percentile formula
        AR_EXP_Reduce(perc);
        _test_ar_func(perc, AR_EXP_Evaluate(expected[i]));
    }
    printf("test_percentile_disc - PASS!\n");
}
int main(int argc, char **argv) {
    AR_RegisterFuncs();
    Agg_RegisterFuncs();

    test_percentile_disc();

    return 0;
}