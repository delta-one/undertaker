/*
 *   boolframwork - boolean framework for undertaker and satyr
 *
 * Copyright (C) 2012 Ralf Hackner <rh@ralf-hackner.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "bool.h"
#include "CNFBuilder.h"
#include "PicosatCNF.h"
#include <iostream>
#include <check.h>
#include <string>

using namespace kconfig;
using namespace std;

START_TEST(buildCNFOr)
{
    BoolExp *e = BoolExp::parseString("x || y");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFAnd)
{
    BoolExp *e = BoolExp::parseString("x && y");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFImplies)
{
    BoolExp *e = BoolExp::parseString("x -> y");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFEqual)
{
    BoolExp *e = BoolExp::parseString("x <-> y");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFNot)
{
    BoolExp *e = BoolExp::parseString("x <-> !y");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFMultiNot0)
{
    BoolExp *e = BoolExp::parseString("!!!x");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    fail_if(!cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFMultiNot1)
{
    BoolExp *e = BoolExp::parseString("x <-> !!!y");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFConst)
{
    BoolExp *e = BoolExp::parseString("(x || 0) && (y && 1)");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",true);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("x",true);
    cnf->pushAssumption("y",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("x",false);
    cnf->pushAssumption("y",false);
    fail_if(cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildCNFComplex0)
{
    BoolExp *e = BoolExp::parseString("a -> (b || !c && d)");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);
    /*
      A B C D  |  A -> (B v (~C & D))
      ---------+---------------------
      1 1 1 1  |    *1 x
      1 1 1 0  |    *1 x
      1 1 0 1  |    *1
      1 1 0 0  |    *1
      1 0 1 1  |    *0 x (shows, wether not does work)
      1 0 1 0  |    *0
      1 0 0 1  |    *1
      1 0 0 0  |    *0 x
      0 1 1 1  |    *1
      0 1 1 0  |    *1
      0 1 0 1  |    *1
      0 1 0 0  |    *1
      0 0 1 1  |    *1
      0 0 1 0  |    *1
      0 0 0 1  |    *1
      0 0 0 0  |    *1
    */

    cnf->pushAssumption("a",true);
    cnf->pushAssumption("b",true);
    cnf->pushAssumption("c",true);
    cnf->pushAssumption("d",true);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("a",true);
    cnf->pushAssumption("b",true);
    cnf->pushAssumption("c",true);
    cnf->pushAssumption("d",false);
    fail_if(!cnf->checkSatisfiable());

    cnf->pushAssumption("a",true);
    cnf->pushAssumption("b",false);
    cnf->pushAssumption("c",true);
    cnf->pushAssumption("d",true);
    fail_if(cnf->checkSatisfiable());

    cnf->pushAssumption("a",true);
    cnf->pushAssumption("b",false);
    cnf->pushAssumption("c",false);
    cnf->pushAssumption("d",false);
    fail_if(cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildAndNull)
{
    BoolExp *e = BoolExp::parseString("A && 0");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);
    fail_if(cnf->checkSatisfiable());

    // now handle consts as free vars
    PicosatCNF *cnf1 = new PicosatCNF();
    CNFBuilder builder1(false,CNFBuilder::FREE);
    builder1.cnf = cnf1;
    builder1.pushClause(e);
    fail_if(!cnf->checkSatisfiable());

} END_TEST;

START_TEST(buildImplNull)
{
    BoolExp *e = BoolExp::parseString("A && (A <-> 0)");
    PicosatCNF *cnf = new PicosatCNF();
    CNFBuilder builder;
    builder.cnf = cnf;
    builder.pushClause(e);
    fail_if(cnf->checkSatisfiable());

    // now handle consts as free vars
    PicosatCNF *cnf1 = new PicosatCNF();
    CNFBuilder builder1(false,CNFBuilder::FREE);
    builder1.cnf = cnf1;
    builder1.pushClause(e);
    fail_if(!cnf->checkSatisfiable());

} END_TEST;


Suite *cond_block_suite(void)
{

    Suite *s  = suite_create("Suite");
    TCase *tc = tcase_create("CNFBuilder");
    tcase_add_test(tc, buildCNFOr);
    tcase_add_test(tc, buildCNFAnd);
    tcase_add_test(tc, buildCNFImplies);
    tcase_add_test(tc, buildCNFEqual);
    tcase_add_test(tc, buildCNFNot);
    tcase_add_test(tc, buildCNFMultiNot0);
    tcase_add_test(tc, buildCNFMultiNot1);
    tcase_add_test(tc, buildCNFComplex0);
    tcase_add_test(tc, buildCNFConst);
    tcase_add_test(tc, buildAndNull);
    tcase_add_test(tc, buildImplNull);
    suite_add_tcase(s, tc);
    return s;
}

int main()
{

    Suite *s = cond_block_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}