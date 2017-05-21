#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIPSIN "clips.in"
#define CLIPSOUT "clips.out"
#define CLIPSCMD ("clips < " CLIPSIN " > " CLIPSOUT)
#define CLIPSPROMPT "CLIPS>"

#define MARKER "STOP_RUN"

#define LINEMAX 4096

#define ISPACE 100

int total_score;

struct win_record {
	char *id;
	int *posx;
	int *posy;
	int count;
	int space;
};

struct fact {
	int id;
	char *name;
	void *data;
};

struct fact_table {
	struct fact_table *next;
	struct fact *fact;
};

struct test {
	int score;
	char *filenames[10];
};

#define EMPTY 0
#define BALL 1
#define MAN 2

struct world {
	char *id;
	int lx, ly;
	int bx, by;
	int **world;
};

struct world_list {
	struct world_list *next;
	struct world *world;
};

struct test all_tests[] = {
	{1, {"tests/t1.txt", NULL}}, {1, {"tests/t2.txt", NULL}},
	{2, {"tests/t3.txt", NULL}}, {3, {"tests/t4.txt", NULL}},
	{3, {"tests/t5.txt", NULL}}, {3, {"tests/t6.txt", NULL}},
	{3, {"tests/t7.txt", NULL}}, {3, {"tests/t8.txt", NULL}},
	{3, {"tests/t9.txt", NULL}}, {3, {"tests/t10.txt", NULL}},
	{3, {"tests/t11.txt", NULL}}, {3, {"tests/t12.txt", NULL}},
	{3, {"tests/t13.txt", NULL}}, {3, {"tests/t14.txt", NULL}},
	{3, {"tests/t15.txt", NULL}}, {3, {"tests/t16.txt", NULL}},
	{3, {"tests/t17.txt", NULL}}, {3, {"tests/t18.txt", NULL}},
	{3, {"tests/t19.txt", NULL}}, {3, {"tests/t20.txt", NULL}},
	{3, {"tests/t21.txt", NULL}}, {3, {"tests/t22.txt", NULL}},
	{3, {"tests/t23.txt", NULL}}, {3, {"tests/t24.txt", NULL}},
	{3, {"tests/t25.txt", NULL}},
	{1, {"tests/t1.txt", "tests/t2.txt", NULL}},
	{4, {"tests/t16.txt", "tests/t17.txt", NULL}},
	{4, {"tests/t18.txt", "tests/t19.txt", "tests/t20.txt",
		    "tests/t21.txt", NULL}},
	{4, {"tests/t1.txt", "tests/t4.txt", "tests/t6.txt", "tests/t8.txt",
		    "tests/t10.txt", "tests/t11.txt", "tests/t14.txt",
		    "tests/t16.txt", "tests/t18.txt", NULL}},
	{4, {"tests/t22.txt", "tests/t23.txt", "tests/t24.txt",
		    "tests/t25.txt", NULL}},
};
#define NUM_TESTS 30

char *passed_tests[] = {"simple", "still-simple", "one-jump-ahead",
	"line-of-jumps", "two-roads", "all-roads-lead-to-rome", "detour",
	"repaired", "round2", "city1", "city3", "city4", "loop1", "loop2",
	"loop4"
};
#define PASSED_TESTS 15

#define INITIAL_FACT(f) (strcmp(f->name, "initial-fact") == 0)
#define WIN_FACT(f) (strcmp(f->name, "win") == 0)

void free_world(struct world *w)
{
	int i;

	free(w->id);
	for (i = 0; i < w->lx; i++)
		free(w->world[i]);
	free(w->world);
	free(w);
}

void print_fact(struct fact *fact)
{
	if (INITIAL_FACT(fact))
		printf("f-%d\t(%s)\n", fact->id, fact->name);
	else if (WIN_FACT(fact)) {
		int i;
		struct win_record *win = fact->data;

		printf("f-%d\t(%s (id %s) (moves", fact->id, fact->name, win->id);
		for (i = 0; i < win->count; i++)
			printf(" %d %d -", win->posx[i], win->posy[i]);
		printf(")))\n");
	}
	else
		printf("f-%d\t%s\n", fact->id, (char*)fact->data);
}

void clear_fact(struct fact *fact)
{
	if (INITIAL_FACT(fact))
		/* do nothing */;
	else if (WIN_FACT(fact)) {
		struct win_record *win = fact->data;
		free(win->posx);
		free(win->posy);
		free(win->id);
		free(win);
	} else
		free(fact->data);
	free(fact->name);
	free(fact);
}

void clear_fact_table(struct fact_table *ft)
{
	struct fact_table *tmp;
	if (!ft)
		return;

	while (ft) {
		clear_fact(ft->fact);
		tmp = ft;
		ft = ft->next;
		free(tmp);
	}
}

char *build_initial_cmd();
char *append_cmd(char *old_cmd, const char *cmd);
struct fact_table *run_clips(char *);

#define TEST_OK 42
/* please keep only win facts */
#define EXTRA_FACTS 43
/* no win fact where there should be one or win fact where there should be none */
#define INCORRECT_WIN_RECORD 44
/* invalid win record */
#define NO_SUCH_PATH 45
/* don't remove initial-fact */
#define NO_INITIAL_FACT 46
/* NO_END_OF_FIELD: ball not at end of line */
#define NO_END_OF_FIELD 47
/* INVALID_PATH_START: path doesn't start at ball pos */
#define INVALID_PATH_START 48
/* unknown id: created by code, not by input */
#define UNKNOWN_ID 49
/* don't create duplicate win solutions */
#define DUPLICATE_FACTS 50

int print_fact_table(struct fact_table *ft, void *unused)
{
	unused = unused;

	while (ft) {
		print_fact(ft->fact);
		ft = ft->next;
	}

	return TEST_OK;
}

int check_for_initial_fact(struct fact_table *ft, void *unused)
{
	unused = unused;

	while (ft)
		if (INITIAL_FACT(ft->fact))
			return TEST_OK;
		else
			ft = ft->next;

	return NO_INITIAL_FACT;
}

int check_for_extra_fact(struct fact_table *ft, void *unused)
{
	unused = unused;

	while (ft)
		if (!INITIAL_FACT(ft->fact) && !WIN_FACT(ft->fact))
			return EXTRA_FACTS;
		else
			ft = ft->next;

	return TEST_OK;
}

int check_unknown_id(struct fact_table *ft, struct world_list *worlds)
{
	int found;
	struct world_list *w;
	struct win_record *wr;

	for (; ft; ft = ft->next) {
		found = 0;
		if (!WIN_FACT(ft->fact))
			continue;

		wr = ft->fact->data;

		for (w = worlds; w; w = w->next) {
			if (strcmp(w->world->id, wr->id))
				continue;
			found = 1;
			break;
		}

		if (!found)
			return UNKNOWN_ID;
	}

	return TEST_OK;
}

int check_incorrect(struct fact_table *ftbl, struct world_list *wl)
{
	int i, found;
	struct win_record *wr;
	struct fact_table *ft;

	/* check all worlds in ft are winnable */
	for (ft = ftbl; ft; ft = ft->next) {
		if (!WIN_FACT(ft->fact))
			continue;

		found = 0;
		wr = ft->fact->data;
		for (i = 0; i < PASSED_TESTS; i++) {
			if (strcmp(passed_tests[i], wr->id))
				continue;
			found = 1;
			break;
		}

		if (!found)
			return INCORRECT_WIN_RECORD;
	}

	/* check worlds in wl and not in ftbl are not winnable */
	for (; wl; wl = wl->next) {
		found = 0;
		for (i = 0; i < PASSED_TESTS && !found; i++)
			if (!strcmp(passed_tests[i], wl->world->id))
				found = 1;

		if (!found)
			continue;

		found = 0;
		for (ft = ftbl; ft && !found; ft = ft->next) {
			if (!WIN_FACT(ft->fact))
				continue;

			wr = ft->fact->data;
			if (!strcmp(wr->id, wl->world->id))
				found = 1;
		}

		if (!found)
			return INCORRECT_WIN_RECORD;
	}

	return TEST_OK;
}

int check_end_of_field(struct fact_table *ftbl, struct world_list *wlst)
{
	struct fact_table *ft;
	struct win_record *wr;
	struct world_list *wl;

	for (ft = ftbl; ft; ft = ft->next) {
		if (!WIN_FACT(ft->fact))
			continue;

		wr = ft->fact->data;

		for (wl = wlst; wl; wl = wl->next)
			if (!strcmp(wl->world->id, wr->id))
				break;
		if (!wl)
			return INCORRECT_WIN_RECORD;

		if (wr->posx[wr->count - 1] != wl->world->lx - 1)
			return NO_END_OF_FIELD;
	}

	return TEST_OK;
}

int check_invalid_start_path(struct fact_table *ftbl, struct world_list *wlst)
{
	struct fact_table *ft;
	struct win_record *wr;
	struct world_list *wl;

	for (ft = ftbl; ft; ft = ft->next) {
		if (!WIN_FACT(ft->fact))
			continue;

		wr = ft->fact->data;

		for (wl = wlst; wl; wl = wl->next)
			if (!strcmp(wl->world->id, wr->id))
				break;
		if (!wl)
			return INCORRECT_WIN_RECORD;

		if (wr->posx[0] != wl->world->bx)
			return INVALID_PATH_START;
		if (wr->posy[0] != wl->world->by)
			return INVALID_PATH_START;
	}

	return TEST_OK;
}

int simulate_world(struct win_record *wr, struct world *world)
{
	int ret = 1, i, dx, dy, dm, j;

	for (i = 1; i < wr->count; i++) {
		dx = wr->posx[i] - world->bx;
		dy = wr->posy[i] - world->by;

		if (abs(dx) == abs(dy))
			dm = abs(dx);
		else if (dx == 0)
			dm = abs(dy);
		else if (dy == 0)
			dm = abs(dx);
		else
			goto out;

		if (!dm)
			goto out;

		dx = dx > 0 ? 1 : dx < 0 ? -1 : 0;
		dy = dy > 0 ? 1 : dy < 0 ? -1 : 0;

		for (j = 1; j < dm; j++) {
			world->bx += dx;
			if (world->bx < 0 || world->bx >= world->lx)
				goto out;
			world->by += dy;
			if (world->by < 0 || world->by >= world->ly)
				goto out;
			if (world->world[world->bx][world->by] != MAN)
				goto out;
			else
				world->world[world->bx][world->by] = EMPTY;
		}

		world->bx += dx;
		if (world->bx < 0 || world->bx >= world->lx)
			goto out;
		world->by += dy;
		if (world->by < 0 || world->by >= world->ly)
			goto out;
		if (world->world[world->bx][world->by] != EMPTY)
			goto out;
	}

	ret = 0;

out:
	return ret;
}

int valid_path(struct fact_table *ftbl, struct world_list *wlst)
{
	struct fact_table *ft;
	struct win_record *wr;
	struct world_list *wl;

	for (ft = ftbl; ft; ft = ft->next) {
		if (!WIN_FACT(ft->fact))
			continue;

		wr = ft->fact->data;

		for (wl = wlst; wl; wl = wl->next)
			if (!strcmp(wl->world->id, wr->id))
				break;
		if (!wl)
			return INCORRECT_WIN_RECORD;

		if (simulate_world(wr, wl->world))
			return NO_SUCH_PATH;
	}

	return TEST_OK;
}

int duplicate_fact(struct fact_table *ftbl)
{
	int retval = 1;
	struct fact_table *ft, *ft1;
	struct win_record *wr, *wr1;

	for (ft = ftbl; ft; ft = ft->next)
		if (!WIN_FACT(ft->fact))
			continue;
		else {
			wr = ft->fact->data;
			for (ft1 = ft->next; ft1; ft1 = ft1->next)
				if (!WIN_FACT(ft1->fact))
					continue;
				else {
					wr1 = ft1->fact->data;
					if (!strcmp(wr->id, wr1->id))
						goto out;
				}
		}

	retval = 0;
out:
	return retval;
}

int test_everything(struct fact_table *ft, void *data)
{
	struct world_list *worlds = data;

	if (check_for_initial_fact(ft, worlds) != TEST_OK)
		return NO_INITIAL_FACT;

	if (check_for_extra_fact(ft, worlds) != TEST_OK)
		return EXTRA_FACTS;

	if (duplicate_fact(ft))
		return DUPLICATE_FACTS;

	if (check_unknown_id(ft, worlds) != TEST_OK)
		return UNKNOWN_ID;

	if (check_incorrect(ft, worlds) != TEST_OK)
		return INCORRECT_WIN_RECORD;

	if (check_end_of_field(ft, worlds) != TEST_OK)
		return NO_END_OF_FIELD;

	if (check_invalid_start_path(ft, worlds) != TEST_OK)
		return INVALID_PATH_START;

	if (valid_path(ft, worlds) != TEST_OK)
		return NO_SUCH_PATH;

	return TEST_OK;
}

void test_results(int tid, char *cmd,
		int (*check_result)(struct fact_table *, void *),
		void *data,
		int score)
{
	struct fact_table *ft = run_clips(cmd);

	printf("Test %02d:", tid);

	if (ft == NULL) {
		printf("..(check syntax / no facts found).............failed [  0 ]\n");
		return;
	}

	if (!check_result)
		goto ok;
		
	switch (check_result (ft, data)) {
	case TEST_OK: goto ok;
	case EXTRA_FACTS:
		printf("..(extra facts found in output)...............failed [  0 ]\n");
		goto out;
	case INCORRECT_WIN_RECORD:
		printf("..(incorrect win record)......................failed [  0 ]\n");
		goto out;
	case NO_SUCH_PATH:
		printf("..(invalid jump list).........................failed [  0 ]\n");
		goto out;
	case NO_INITIAL_FACT:
		printf("..(don't remove initial fact).................failed [  0 ]\n");
		goto out;
	case NO_END_OF_FIELD:
		printf("..(ball not on end line)......................failed [  0 ]\n");
		goto out;
	case INVALID_PATH_START:
		printf("..(path doesn't start at ball pos)............failed [  0 ]\n");
		goto out;
	case UNKNOWN_ID:
		printf("..(this world id does not exist)..............failed [  0 ]\n");
		goto out;
	case DUPLICATE_FACTS:
		printf("..(multiple win records for same world).......failed [  0 ]\n");
		goto out;
	}

	printf("..(unknown error, contact team)...............failed [  0 ]\n");

out:
	clear_fact_table(ft);
	return;
ok:
	printf("..............................................passed [ %2d ]\n", score);
	total_score += score;
	goto out;
}

struct world *read_test_file(const char *filename)
{
	struct world *w = calloc(1, sizeof(*w));
	FILE *f = fopen(filename, "r");
	char s[LINEMAX];
	int i, j;

	if (!w) {
		perror("Cannot open test file");
		exit(EXIT_FAILURE);
	}

	fgets(s, LINEMAX, f); /* test id */
	s[strlen(s) - 1] = 0;
	w->id = strdup(s);

	fgets(s, LINEMAX, f); /* sx sy */
	if (sscanf(s, "%d%d", &w->lx, &w->ly) != 2) {
		fprintf(stderr, "Invalid test file");
		exit(EXIT_FAILURE);
	}

	w->world = calloc(w->lx, sizeof(w->world[0]));
	for (i = 0; i < w->lx; i++) {
		w->world[i] = calloc(w->ly, sizeof(w->world[0][0]));
		fgets(s, LINEMAX, f);
		for (j = 0; j < w->ly; j++)
			switch(s[j]) {
			case 'B':
				w->bx = i;
				w->by = j;
				
				break;
			case 'M':
				w->world[i][j] = MAN;
				break;
			}
	}

	fclose(f);
	return w;
}

char *add_world(char *cmd, struct world *w)
{
	char *new_cmd = NULL;
	char *men = NULL;
	char *tmp;
	int i, j;

	asprintf(&men, "(men");
	for (i = 0; i < w->lx; i++)
		for (j = 0; j < w->ly; j++)
			if (w->world[i][j] == MAN) {
				tmp = strdup(men);
				free(men);
				asprintf(&men, "%s %d %d -", tmp, i, j);
				free(tmp);
			}
	tmp = strdup(men);
	free(men);
	asprintf(&men, "%s)", tmp);
	free(tmp);

	asprintf(&new_cmd, "(assert (world (id %s) (limit %d %d) (ball %d %d) %s))",
			w->id, w->lx, w->ly, w->bx, w->by, men);
	free(men);

	cmd = append_cmd(cmd, new_cmd);
	free(new_cmd);

	return cmd;
}

char *build_test_cmd(struct world_list *wl)
{
	char *cmd = build_initial_cmd();

	while (wl) {
		cmd = add_world(cmd, wl->world);
		wl = wl->next;
	}

	return cmd;
}

void test(int ix) {
	struct world_list *wl = NULL, *tmp;
	struct test test = all_tests[ix];
	char *cmd;
	int i;

	wl = calloc(1, sizeof(*wl));
	wl->world = read_test_file(test.filenames[0]);
	for (i = 1; test.filenames[i]; i++) {
		tmp = calloc(1, sizeof(*tmp));
		tmp->world = read_test_file(test.filenames[i]);
		tmp->next = wl;
		wl = tmp;
	}

	cmd = build_test_cmd(wl);
	test_results(4 + ix, cmd, test_everything, wl, test.score); /* syntax check */

	while (wl) {
		tmp = wl->next;
		free_world(wl->world);
		free(wl);
		wl = tmp;
	}
}

int main(int argc, char **argv)
{
	int i;

	if (argc > 1 && sscanf(argv[1], "%d", &i) && i > 0 && i < NUM_TESTS + 4) {
		switch (i) {
		case 1: test_results(1, build_initial_cmd(), NULL, NULL, 1);
			break;
		case 2: test_results(2, build_initial_cmd(), check_for_initial_fact, NULL, 1);
			break;
		case 3: test_results(3, build_initial_cmd(), check_for_extra_fact, NULL, 1);
			break;
		default: test(i - 4);
		}
		return 0;
	} else if (argc > 1) {
		fprintf(stderr, "Garbage on command line args, ignoring..\n");
	}

	test_results(1, build_initial_cmd(), NULL, NULL, 1); /* syntax check */
	test_results(2, build_initial_cmd(), check_for_initial_fact, NULL, 1);
	test_results(3, build_initial_cmd(), check_for_extra_fact, NULL, 1);

	for (i = 0; i < NUM_TESTS; i++)
		test(i);

	printf("==============================================================\n");
	printf("Score: %2d / 90\n", total_score);
	return 0;
}

char *empty_cmd()
{
	return strdup("");
}

char *append_cmd(char *old_cmd, const char *cmd)
{
	char *s = NULL;

	if (asprintf(&s, "%s%s\n", old_cmd, cmd) < 0) {
		fprintf(stderr, "Cannot append command");
		exit(EXIT_FAILURE);
	}

	free(old_cmd);
	return s;
}

char *build_initial_cmd()
{
	char *s = empty_cmd();

	s = append_cmd(s, "(clear)");
	s = append_cmd(s, "(load templates.clp)");
	s = append_cmd(s, "(load phutball.clp)");
	s = append_cmd(s, "(reset)");
	return s;
}

char *close_cmd(char *cmd)
{

	cmd = append_cmd(cmd, "(run)");
	cmd = append_cmd(cmd, MARKER);
	cmd = append_cmd(cmd, "(facts)");
	cmd = append_cmd(cmd, "(exit)");
	return cmd;
}

void write_cmd(char *cmds)
{
	FILE *f = fopen(CLIPSIN, "w");
	if (!f) {
		fprintf(stderr, "Cannot open CLIPS input");
		exit(EXIT_FAILURE);
	}

	cmds = close_cmd(cmds);

	fprintf(f, "%s", cmds);
	fclose(f);
	free(cmds);
}

int parse_win_fact(struct fact *fact)
{
	int retval = 1;
	char *tmp = strtok(NULL, "( )"); /* should be id */
	struct win_record *win = calloc(1, sizeof(*win));

	tmp = strtok(NULL, "( )");
	win->id = strdup(tmp);

	win->space = ISPACE;
	win->posx = calloc(win->space, sizeof(win->posx[0]));
	win->posy = calloc(win->space, sizeof(win->posy[0]));
	fact->data = win;

	tmp = strtok(NULL, "( )"); /* should be moves */

	while (tmp) {
		if (win->count == win->space) {
			win->space *= 2;
			win->posx = realloc(win->posx, win->space *
					sizeof(win->posx[0]));
			win->posy = realloc(win->posy, win->space *
					sizeof(win->posy[0]));
		}
		tmp = strtok(NULL, "( )"); /* x */
		if (!tmp)
			break;
		if (sscanf(tmp, "%d", &(win->posx[win->count])) != 1)
			goto out;
		tmp = strtok(NULL, "( )"); /* y */
		if (!tmp || sscanf(tmp, "%d", &(win->posy[win->count])) != 1)
			goto out;
		tmp = strtok(NULL, "( )"); /* - */
		if (!tmp || strcmp(tmp, "-"))
			goto out;
		win->count++;
	}
	retval = 0;
out:
	return retval;
}

int parse_fact(const char *s, struct fact *fact)
{
	char *tmp, *scpy;
	int retval = 1;

	asprintf(&scpy, "%s", s);

	tmp = strtok(scpy, "( )");
	if (!tmp)
		goto out;

	/* switch by fact name */
	asprintf(&fact->name, "%s", tmp);
	if (INITIAL_FACT(fact))
		retval = 0;
	else if (WIN_FACT(fact))
		retval = parse_win_fact(fact);
	else {
		char *fact_data;
		asprintf(&fact_data, "%s", s);
		fact->data = fact_data;
		retval = 0;
	}

out:
	free(scpy);
	return retval;
}

/* ugly :( */
struct fact_table *read_fact(struct fact_table *ft, const char *s)
{
	int id;
	char *tmp;
	struct fact *fact;
	struct fact_table *ftelem;

	if (s[0] != 'f' && s[1] != '-')
		goto bad_fact;
	s+=2;

	if (sscanf(s, "%d", &id) != 1)
		goto bad_fact;

	fact = calloc(1, sizeof(*fact));
	fact->id = id;
	ftelem = calloc(1, sizeof(*ftelem));
	ftelem->fact = fact;
	if (ft)
		ftelem->next = ft;
	ft = ftelem;

	tmp = strchr(s, '(');
	if (!tmp)
		goto bad_fact;
	s = tmp;

	if (parse_fact(s, fact))
		goto bad_fact;
	return ft;

bad_fact:
	fprintf(stderr, "Not a valid fact: %s\n", s);
	clear_fact_table(ft);
	return NULL;
}

/* ugly again :( */
struct fact_table *read_output()
{
	char s[LINEMAX], *tmp;
	FILE *f = fopen(CLIPSOUT, "r");
	struct fact_table *ft = NULL;

	if (!f) {
		fprintf(stderr, "Cannot open CLIPS output");
		exit(EXIT_FAILURE);
	}

	fgets(s, LINEMAX, f); /* skip CLIPS header */
	while (fgets(s, LINEMAX, f)) {
		int l = strlen(s);
		if (s[l - 1] != '\n' && l >= LINEMAX - 5) {
			fprintf(stderr, "Line too long. Skipped\n");
			continue;
		}
		s[l - 1] = 0;

		if (strstr(s, "FALSE"))
			goto out;

		if (strstr(s, MARKER))
			break;
	}

	while (fgets(s, LINEMAX, f)) {
		int l = strlen(s);
		if (s[l - 1] != '\n' && l >= LINEMAX - 5) {
			fprintf(stderr, "Line too long. Skipped\n");
			continue;
		}
		s[l - 1] = 0;

		tmp = s;
		while (strstr(tmp, CLIPSPROMPT) == tmp) {
			tmp += strlen(CLIPSPROMPT);
			if (tmp[0] == ' ')
				tmp++;
		}

		if (strstr(tmp, CLIPSPROMPT)) {
			fprintf(stderr, "Malformed line. skipped\n");
			continue;
		}

		if (strstr(tmp, "For a total of"))
			break;

		if (strlen(tmp) == 0)
			continue;

		ft = read_fact(ft, tmp);
		if (!ft)
			goto out;
	}

out:
	fclose(f);
	return ft;
}

struct fact_table *run_clips(char *cmds)
{
	write_cmd(cmds);

	if (system(CLIPSCMD)) {
		fprintf(stderr, "Cannot start CLIPS environment");
		exit(EXIT_FAILURE);
	}

	return read_output();
}
