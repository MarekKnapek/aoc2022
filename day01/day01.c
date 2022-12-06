#define _CRT_SECURE_NO_WARNINGS


#include <stddef.h>
#include <assert.h>
#include <stdio.h>


int mk_aoc_find(unsigned char const needle, unsigned char const* const haystack, int const len, int* const position)
{
	int ret;
	int i;

	assert(haystack);
	assert(len >= 0);
	assert(position);

	ret = len;
	for(i = 0; i != len; ++i)
	{
		if(haystack[i] == needle)
		{
			ret = i;
			break;
		}
	}
	*position = ret;
	return 0;
}

int mk_aoc_parse_int_line(FILE* const file, int* const eof, int* const empty, int* const num)
{
	static char const s_digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	size_t read;
	unsigned char elem;
	int err;
	int digit;
	int ret;

	assert(sizeof(s_digits) / sizeof(s_digits[0]) == 10);
	assert(file);
	assert(eof);
	assert(empty);
	assert(num);

	if(ferror(file) != 0)
	{
		return ((int)(__LINE__));
	}
	if(feof(file) != 0)
	{
		*eof = 1;
		return 0;
	}
	read = fread(&elem, 1, 1, file);
	if(read != 1)
	{
		if(ferror(file) != 0)
		{
			return ((int)(__LINE__));
		}
		if(feof(file) != 0)
		{
			*eof = 1;
			return 0;
		}
		return ((int)(__LINE__));
	}
	if(elem == 0x0a || elem == 0x0d)
	{
		*eof = 0;
		*empty = 1;
		return 0;
	}
	err = mk_aoc_find(elem, s_digits, 10, &digit);
	if(err != 0) return err;
	if(digit == 10) return ((int)(__LINE__));
	ret = digit;
	for(;;)
	{
		read = fread(&elem, 1, 1, file);
		if(read != 1)
		{
			if(ferror(file) != 0)
			{
				return ((int)(__LINE__));
			}
			if(feof(file) != 0)
			{
				break;
			}
			return ((int)(__LINE__));
		}
		if(elem == 0x0a || elem == 0x0d)
		{
			break;
		}
		err = mk_aoc_find(elem, s_digits, 10, &digit);
		if(err != 0) return err;
		if(digit == 10) return ((int)(__LINE__));
		ret = ret * 10 + digit;
	}
	*eof = 0;
	*empty = 0;
	*num = ret;
	return 0;
}

int mk_aoc2022_day01(char const* const input, int* const out)
{
	FILE* f;
	int err;
	int eof;
	int empty;
	int calories;
	int biggest;
	int current;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	/* first line of first elf */
	err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
	if(err != 0) return err;
	if(eof != 0) return ((int)(__LINE__));
	if(empty != 0) return ((int)(__LINE__));
	biggest = calories;
	for(;;)
	{
		/* rest lines of first elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		biggest += calories;
	}
	for(;;)
	{
		/* first line of rest elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		current = calories;
		for(;;)
		{
			/* rest lines of rest elf */
			err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
			if(err != 0) return err;
			if(eof != 0) goto break2;
			if(empty != 0) break;
			current += calories;
		}
		if(current > biggest) biggest = current;
	}
	break2:;
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = biggest;
	return 0;
}

int mk_aoc2022_day01b(char const* const input, int* const out)
{
	int biggest[3];
	FILE* f;
	int err;
	int eof;
	int empty;
	int calories;
	int current;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	biggest[1] = 0;
	biggest[2] = 0;
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	/* first line of first elf */
	err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
	if(err != 0) return err;
	if(eof != 0) return ((int)(__LINE__));
	if(empty != 0) return ((int)(__LINE__));
	biggest[0] = calories;
	for(;;)
	{
		/* rest lines of first elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		biggest[0] += calories;
	}
	for(;;)
	{
		/* first line of rest elf */
		err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
		if(err != 0) return err;
		if(eof != 0) goto break2;
		if(empty != 0) break;
		current = calories;
		for(;;)
		{
			/* rest lines of rest elf */
			err = mk_aoc_parse_int_line(f, &eof, &empty, &calories);
			if(err != 0) return err;
			if(eof != 0) goto break2;
			if(empty != 0) break;
			current += calories;
		}
		if(current > biggest[0])
		{
			biggest[2] = biggest[1];
			biggest[1] = biggest[0];
			biggest[0] = current;
		}
		else if(current > biggest[1])
		{
			biggest[2] = biggest[1];
			biggest[1] = current;
		}
		else if(current > biggest[2])
		{
			biggest[2] = current;
		}
	}
	break2:;
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = biggest[0] + biggest[1] + biggest[2];
	return 0;
}

enum game_e
{
	game_rock,
	game_paper,
	game_scissors
};

enum game_outcome_e
{
	game_loose,
	game_draw,
	game_win
};

int mk_aoc_parse_game_strategy(FILE* const file, int* const eof, enum game_e* const oponent, enum game_e* const me)
{
	size_t read;
	unsigned char elems[4];

	assert(file);
	assert(eof);
	assert(oponent);
	assert(me);

	if(ferror(file) != 0)
	{
		return ((int)(__LINE__));
	}
	if(feof(file) != 0)
	{
		*eof = 1;
		return 0;
	}
	read = fread(elems, 4, 1, file);
	if(read != 1)
	{
		if(ferror(file) != 0)
		{
			return ((int)(__LINE__));
		}
		if(feof(file) != 0)
		{
			*eof = 1;
			return 0;
		}
		return ((int)(__LINE__));
	}
	switch(((char)(elems[0])))
	{
		case 'A': *oponent = game_rock; break;
		case 'B': *oponent = game_paper; break;
		case 'C': *oponent = game_scissors; break;
		default: return ((int)(__LINE__)); break;
	}
	if(!(elems[1] == 0x20)) return ((int)(__LINE__));
	switch(((char)(elems[2])))
	{
		case 'X': *me = game_rock; break;
		case 'Y': *me = game_paper; break;
		case 'Z': *me = game_scissors; break;
		default: return ((int)(__LINE__)); break;
	}
	if(!(elems[3] == 0x0a)) return ((int)(__LINE__));
	*eof = 0;
	return 0;
}

int mk_aoc_parse_game_strategyb(FILE* const file, int* const eof, enum game_e* const oponent, enum game_outcome_e* const outcome)
{
	size_t read;
	unsigned char elems[4];

	assert(file);
	assert(eof);
	assert(oponent);
	assert(outcome);

	if(ferror(file) != 0)
	{
		return ((int)(__LINE__));
	}
	if(feof(file) != 0)
	{
		*eof = 1;
		return 0;
	}
	read = fread(elems, 4, 1, file);
	if(read != 1)
	{
		if(ferror(file) != 0)
		{
			return ((int)(__LINE__));
		}
		if(feof(file) != 0)
		{
			*eof = 1;
			return 0;
		}
		return ((int)(__LINE__));
	}
	switch(((char)(elems[0])))
	{
		case 'A': *oponent = game_rock; break;
		case 'B': *oponent = game_paper; break;
		case 'C': *oponent = game_scissors; break;
		default: return ((int)(__LINE__)); break;
	}
	if(!(elems[1] == 0x20)) return ((int)(__LINE__));
	switch(((char)(elems[2])))
	{
		case 'X': *outcome = game_loose; break;
		case 'Y': *outcome = game_draw; break;
		case 'Z': *outcome = game_win; break;
		default: return ((int)(__LINE__)); break;
	}
	if(!(elems[3] == 0x0a)) return ((int)(__LINE__));
	*eof = 0;
	return 0;
}

int mk_aoc_day02_game_win_a(enum game_e a, enum game_e b)
{
	if
	(
		(a == game_rock && b == game_scissors) ||
		(a == game_paper && b == game_rock) ||
		(a == game_scissors && b == game_paper)
	)
	{
		return 1;
	}
	return 0;
}

int mk_aoc2022_day02a(char const* const input, int* const out)
{
	int score;
	FILE* f;
	int err;
	int eof;
	enum game_e oponent;
	enum game_e me;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	score = 0;
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		err = mk_aoc_parse_game_strategy(f, &eof, &oponent, &me);
		if(err != 0) return err;
		if(eof != 0) break;
		switch(me)
		{
			case game_rock: score += 1; break;
			case game_paper: score += 2; break;
			case game_scissors: score += 3; break;
		}
		if(mk_aoc_day02_game_win_a(me, oponent) != 0)
		{
			score += 6;
		}
		else if(mk_aoc_day02_game_win_a(oponent, me) != 0)
		{
			score += 0;
		}
		else
		{
			score += 3;
		}
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = score;
	return 0;
}

int mk_aoc2022_day02b(char const* const input, int* const out)
{
	int score;
	FILE* f;
	int err;
	int eof;
	enum game_e oponent;
	enum game_e me;
	enum game_outcome_e outcome;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	score = 0;
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		err = mk_aoc_parse_game_strategyb(f, &eof, &oponent, &outcome);
		if(err != 0) return err;
		if(eof != 0) break;
		switch(outcome)
		{
			case game_loose:
				switch(oponent)
				{
					case game_rock: me = game_scissors; break;
					case game_paper: me = game_rock; break;
					case game_scissors: me = game_paper; break;
				}
			break;
			case game_draw:
				me = oponent;
			break;
			case game_win:
				switch(oponent)
				{
					case game_rock: me = game_paper; break;
					case game_paper: me = game_scissors; break;
					case game_scissors: me = game_rock; break;
				}
			break;
		}
		switch(me)
		{
			case game_rock: score += 1; break;
			case game_paper: score += 2; break;
			case game_scissors: score += 3; break;
		}
		switch(outcome)
		{
			case game_loose: score += 0; break;
			case game_draw: score += 3; break;
			case game_win: score += 6; break;
		}
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = score;
	return 0;
}


#include <stdio.h>

int main(void)
{
	int err;
	int ret;

	printf("%s\n", "Day 1");
	err = mk_aoc2022_day01("input01a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day01("input01b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day01b("input01b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	printf("%s\n", "Day 2");
	err = mk_aoc2022_day02a("input02a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day02a("input02b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day02b("input02b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	return 0;
}
