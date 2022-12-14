#define _CRT_SECURE_NO_WARNINGS


#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void crash(void)
{
	int volatile* volatile ptr;

	ptr = NULL;
	*ptr = 0;
}

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


struct rucksack_s
{
	int m_count;
	int m_capacity;
	char* m_elements;
};

void mk_aoc2022_day03_rucksack_construct(struct rucksack_s* r)
{
	assert(r);

	r->m_count = 0;
	r->m_capacity = 0;
	r->m_elements = NULL;
}

void mk_aoc2022_day03_rucksack_destruct(struct rucksack_s* r)
{
	assert(r);

	free(r->m_elements);
}

void mk_aoc2022_day03_rucksack_append(struct rucksack_s* r, char ch)
{
	char* newelems;

	if(r->m_count == r->m_capacity)
	{
		r->m_capacity *= 2;
		if(r->m_capacity == 0)
		{
			r->m_capacity = 8;
		}
		newelems = malloc(r->m_capacity * sizeof(char));
		if(newelems == NULL) crash();
		memcpy(newelems, r->m_elements, r->m_count * sizeof(char));
		free(r->m_elements);
		r->m_elements = newelems;
	}
	r->m_elements[r->m_count] = ch;
	++r->m_count;
}

int mk_aoc2022_day03_parse_rucksack(FILE* f, struct rucksack_s* r)
{
	size_t read;
	char ch;

	assert(f);
	assert(r);

	r->m_count = 0;
	if(ferror(f) != 0)
	{
		return ((int)(__LINE__));
	}
	if(feof(f) != 0)
	{
		return 0;
	}
	for(;;)
	{
		read = fread(&ch, 1, 1, f);
		if(read != 1)
		{
			if(ferror(f) != 0)
			{
				return ((int)(__LINE__));
			}
			if(feof(f) != 0)
			{
				break;
			}
			return ((int)(__LINE__));
		}
		if(ch == 0x0a)
		{
			break;
		}
		assert
		(
			(ch >= 'a' && ch <= 'z') ||
			(ch >= 'A' && ch <= 'Z')
		);
		mk_aoc2022_day03_rucksack_append(r, ch);
	}
	if(r->m_count % 2 != 0) return ((int)(__LINE__));
	return 0;
}

int mk_aoc2022_day03_find_bad_item(struct rucksack_s* r, char* ch)
{
	int i;
	int j;

	assert(r);
	assert(r->m_count % 2 == 0);
	assert(ch);

	for(i = 0; i != r->m_count / 2; ++i)
	{
		for(j = r->m_count / 2; j != r->m_count; ++j)
		{
			if(r->m_elements[i] == r->m_elements[j])
			{
				goto break2;
			}
		}
	}
	return ((int)(__LINE__));
	break2:;
	*ch = r->m_elements[i];
	return 0;
}

int mk_aoc2022_day03b_find_common_item(struct rucksack_s* r0, struct rucksack_s* r1, struct rucksack_s* r2, char* item)
{
	int i;
	int j;
	int k;

	assert(r0);
	assert(r1);
	assert(r2);
	assert(item);

	for(i = 0; i != r0->m_count; ++i)
	{
		for(j = 0; j != r1->m_count; ++j)
		{
			if(r0->m_elements[i] == r1->m_elements[j])
			{
				for(k = 0; k != r2->m_count; ++k)
				{
					if(r0->m_elements[i] == r2->m_elements[k])
					{
						goto break3;
					}
				}
			}
		}
	}
	return ((int)(__LINE__));
	break3:;
	*item = r0->m_elements[i];
	return 0;
}

int mk_aoc2022_day03_score_item(char ch)
{
	if(ch >= 'a' && ch <= 'z')
	{
		return ch - 'a' + 1;
	}
	else
	{
		return ch - 'A' + 1 + 26;
	}
}

int mk_aoc2022_day03a(char const* const input, int* const out)
{
	int score;
	struct rucksack_s r;
	FILE* f;
	int err;
	char item;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	score = 0;
	mk_aoc2022_day03_rucksack_construct(&r);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		err = mk_aoc2022_day03_parse_rucksack(f, &r);
		if(err != 0) return err;
		if(r.m_count == 0) break;
		err = mk_aoc2022_day03_find_bad_item(&r, &item);
		if(err != 0) return err;
		score += mk_aoc2022_day03_score_item(item);
	}
	mk_aoc2022_day03_rucksack_destruct(&r);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = score;
	return 0;
}

int mk_aoc2022_day03b(char const* const input, int* const out)
{
	int score;
	struct rucksack_s r[3];
	FILE* f;
	int err;
	char item;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	score = 0;
	mk_aoc2022_day03_rucksack_construct(&r[0]);
	mk_aoc2022_day03_rucksack_construct(&r[1]);
	mk_aoc2022_day03_rucksack_construct(&r[2]);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		err = mk_aoc2022_day03_parse_rucksack(f, &r[0]);
		if(err != 0) return err;
		if(r[0].m_count == 0) break;
		err = mk_aoc2022_day03_parse_rucksack(f, &r[1]);
		if(err != 0) return err;
		if(r[1].m_count == 0) return ((int)(__LINE__));
		err = mk_aoc2022_day03_parse_rucksack(f, &r[2]);
		if(err != 0) return err;
		if(r[2].m_count == 0) return ((int)(__LINE__));
		err = mk_aoc2022_day03b_find_common_item(&r[0], &r[1], &r[2], &item);
		if(err != 0) return err;
		score += mk_aoc2022_day03_score_item(item);
	}
	mk_aoc2022_day03_rucksack_destruct(&r[2]);
	mk_aoc2022_day03_rucksack_destruct(&r[1]);
	mk_aoc2022_day03_rucksack_destruct(&r[0]);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = score;
	return 0;
}


int mk_aoc2022_day04a(char const* const input, int* const out)
{
	int fully_overlapped_count;
	int scanned;
	int section1start;
	int section1end;
	int section2start;
	int section2end;
	FILE* f;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	fully_overlapped_count = 0;
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		scanned = fscanf(f, "%d-%d,%d-%d\x0a", &section1start, &section1end, &section2start, &section2end);
		if(!(scanned == 4))
		{
			if(ferror(f) != 0)
			{
				return ((int)(__LINE__));
			}
			if(feof(f) != 0)
			{
				break;
			}
			return ((int)(__LINE__));
		}
		if
		(
			(section1start >= section2start && section1end <= section2end) ||
			(section2start >= section1start && section2end <= section1end)
		)
		{
			++fully_overlapped_count;
		}
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = fully_overlapped_count;
	return 0;
}

int mk_aoc2022_day04b(char const* const input, int* const out)
{
	int partially_overlapped_count;
	int scanned;
	int section1start;
	int section1end;
	int section2start;
	int section2end;
	FILE* f;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	partially_overlapped_count = 0;
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		scanned = fscanf(f, "%d-%d,%d-%d\x0a", &section1start, &section1end, &section2start, &section2end);
		if(!(scanned == 4))
		{
			if(ferror(f) != 0)
			{
				return ((int)(__LINE__));
			}
			if(feof(f) != 0)
			{
				break;
			}
			return ((int)(__LINE__));
		}
		if
		(
			(section1start >= section2start && section1start <= section2end) ||
			(section2start >= section1start && section2start <= section1end)
		)
		{
			++partially_overlapped_count;
		}
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = partially_overlapped_count;
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

	printf("%s\n", "Day 3");
	err = mk_aoc2022_day03a("input03a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day03a("input03b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day03b("input03b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	printf("%s\n", "Day 4");
	err = mk_aoc2022_day04a("input04a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day04a("input04b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day04b("input04b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	return 0;
}
