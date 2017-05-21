/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x8ddf5b5d */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "time=";
static const char *ng1 = " ";
static const char *ng2 = "A=%d B=%d S=%b M=%b => Sum=%d Cout=%b PG=%b GG=%b";
static const char *ng3 = "/../mnt/hgfs/Dropbox/ReDEnergy/CN/Tema2/test.v";
static int ng4[] = {0, 0};
static int ng5[] = {1, 0};
static unsigned int ng6[] = {15033U, 0U};
static unsigned int ng7[] = {4658U, 0U};
static unsigned int ng8[] = {6U, 0U};
static unsigned int ng9[] = {11U, 0U};
static unsigned int ng10[] = {12345U, 0U};
static unsigned int ng11[] = {11938U, 0U};

void Monitor_87_2(char *);
void Monitor_87_2(char *);


static void Monitor_87_2_Func(char *t0)
{
    char t1[16];
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t6;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    char *t16;
    char *t17;
    char *t18;
    char *t19;

LAB0:    t2 = xsi_vlog_time(t1, 1000.0000000000000, 1000.0000000000000);
    xsi_vlogfile_write(0, 0, 3, ng0, 2, t0, (char)118, t1, 64);
    xsi_vlogfile_write(0, 0, 3, ng1, 1, t0);
    t3 = (t0 + 1288);
    t4 = (t3 + 36U);
    t5 = *((char **)t4);
    t6 = (t0 + 1380);
    t7 = (t6 + 36U);
    t8 = *((char **)t7);
    t9 = (t0 + 1196);
    t10 = (t9 + 36U);
    t11 = *((char **)t10);
    t12 = (t0 + 1472);
    t13 = (t12 + 36U);
    t14 = *((char **)t13);
    t15 = (t0 + 600U);
    t16 = *((char **)t15);
    t15 = (t0 + 876U);
    t17 = *((char **)t15);
    t15 = (t0 + 692U);
    t18 = *((char **)t15);
    t15 = (t0 + 784U);
    t19 = *((char **)t15);
    xsi_vlogfile_write(1, 0, 3, ng2, 9, t0, (char)118, t5, 16, (char)118, t8, 16, (char)118, t11, 4, (char)118, t14, 1, (char)118, t16, 16, (char)118, t17, 1, (char)118, t18, 1, (char)118, t19, 1);

LAB1:    return;
}

static void Initial_55_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;

LAB0:    t1 = (t0 + 2076U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(55, ng3);

LAB4:    xsi_set_current_line(57, ng3);
    t2 = ((char*)((ng4)));
    t3 = (t0 + 1196);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 4);
    xsi_set_current_line(58, ng3);
    t2 = ((char*)((ng4)));
    t3 = (t0 + 1288);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(59, ng3);
    t2 = ((char*)((ng4)));
    t3 = (t0 + 1380);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(60, ng3);
    t2 = ((char*)((ng4)));
    t3 = (t0 + 1472);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(61, ng3);
    t2 = ((char*)((ng4)));
    t3 = (t0 + 1564);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(64, ng3);
    t2 = (t0 + 1976);
    xsi_process_wait(t2, 100000LL);
    *((char **)t1) = &&LAB5;

LAB1:    return;
LAB5:    xsi_set_current_line(67, ng3);
    t2 = ((char*)((ng5)));
    t3 = (t0 + 1472);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(68, ng3);
    t2 = ((char*)((ng6)));
    t3 = (t0 + 1288);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(69, ng3);
    t2 = ((char*)((ng7)));
    t3 = (t0 + 1380);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(73, ng3);
    t2 = (t0 + 1976);
    xsi_process_wait(t2, 30000LL);
    *((char **)t1) = &&LAB6;
    goto LAB1;

LAB6:    xsi_set_current_line(73, ng3);
    t3 = ((char*)((ng8)));
    t4 = (t0 + 1196);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    xsi_set_current_line(75, ng3);
    t2 = (t0 + 1976);
    xsi_process_wait(t2, 30000LL);
    *((char **)t1) = &&LAB7;
    goto LAB1;

LAB7:    xsi_set_current_line(75, ng3);
    t3 = ((char*)((ng9)));
    t4 = (t0 + 1196);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 4);
    xsi_set_current_line(77, ng3);
    t2 = (t0 + 1976);
    xsi_process_wait(t2, 30000LL);
    *((char **)t1) = &&LAB8;
    goto LAB1;

LAB8:    xsi_set_current_line(77, ng3);
    t3 = ((char*)((ng4)));
    t4 = (t0 + 1472);
    xsi_vlogvar_assign_value(t4, t3, 0, 0, 1);
    xsi_set_current_line(77, ng3);
    t2 = ((char*)((ng5)));
    t3 = (t0 + 1564);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(78, ng3);
    t2 = ((char*)((ng8)));
    t3 = (t0 + 1196);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 4);
    xsi_set_current_line(79, ng3);
    t2 = ((char*)((ng10)));
    t3 = (t0 + 1288);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    xsi_set_current_line(80, ng3);
    t2 = ((char*)((ng11)));
    t3 = (t0 + 1380);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 16);
    goto LAB1;

}

static void Initial_85_1(char *t0)
{

LAB0:    xsi_set_current_line(85, ng3);

LAB2:    xsi_set_current_line(87, ng3);
    Monitor_87_2(t0);

LAB1:    return;
}

void Monitor_87_2(char *t0)
{
    char *t1;
    char *t2;

LAB0:    t1 = (t0 + 2248);
    t2 = (t0 + 2528);
    xsi_vlogfile_monitor((void *)Monitor_87_2_Func, t1, t2);

LAB1:    return;
}


extern void work_m_00000000000080534063_1985558087_init()
{
	static char *pe[] = {(void *)Initial_55_0,(void *)Initial_85_1,(void *)Monitor_87_2};
	xsi_register_didat("work_m_00000000000080534063_1985558087", "isim/test_isim_beh.exe.sim/work/m_00000000000080534063_1985558087.didat");
	xsi_register_executes(pe);
}
