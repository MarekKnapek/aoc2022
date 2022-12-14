#define _CRT_SECURE_NO_WARNINGS


#include <assert.h>
#include <limits.h>
#include <stddef.h>
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


struct line_s
{
	int m_count;
	int m_capacity;
	char* m_elements;
};

void mk_aoc2022_day05_line_construct(struct line_s* l)
{
	assert(l);

	l->m_count = 0;
	l->m_capacity = 0;
	l->m_elements = NULL;
}

void mk_aoc2022_day05_line_destruct(struct line_s* l)
{
	assert(l);

	free(l->m_elements);
}

void mk_aoc2022_day05_line_append(struct line_s* l, char ch)
{
	char* newelems;

	if(l->m_count == l->m_capacity)
	{
		l->m_capacity *= 2;
		if(l->m_capacity == 0)
		{
			l->m_capacity = 8;
		}
		newelems = malloc(l->m_capacity * sizeof(char));
		if(newelems == NULL) crash();
		memcpy(newelems, l->m_elements, l->m_count * sizeof(char));
		free(l->m_elements);
		l->m_elements = newelems;
	}
	l->m_elements[l->m_count] = ch;
	++l->m_count;
}

int mk_aoc2022_day05_line_read(FILE* f, struct line_s* l)
{
	size_t read;
	char ch;

	assert(f);
	assert(l);

	l->m_count = 0;
	if(ferror(f) != 0)
	{
		return ((int)(__LINE__));
	}
	if(feof(f) != 0)
	{
		goto end;
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
		mk_aoc2022_day05_line_append(l, ch);
	}
	end:;
	mk_aoc2022_day05_line_append(l, '\0');
	--l->m_count;
	return 0;
}

int mk_aoc2022_day05_line_is_numbers(struct line_s* l)
{
	assert(l);

	return l->m_count >= 2 && l->m_elements[0] == ' ' && l->m_elements[1] == '1';
}

void mk_aoc2022_day05_line_reverse(struct line_s* l)
{
	int i;
	char tmp;

	assert(l);

	for(i = 0; i != l->m_count / 2; ++i)
	{
		tmp = l->m_elements[i];
		l->m_elements[i] = l->m_elements[l->m_count - 1 - i];
		l->m_elements[l->m_count - 1 - i] = tmp;
	}
}

int mk_aoc2022_day05_line_extract_crates(struct line_s* l)
{
	int n;
	int i;

	assert(l);

	if((l->m_count + 1) % 4 != 0) return ((int)(__LINE__));
	n = (l->m_count + 1) / 4;
	for(i = 0; i != n; ++i)
	{
		l->m_elements[i] = l->m_elements[i * 4 + 1];
	}
	l->m_count = n;
	return 0;
}

int mk_aoc2022_day05(char const* const input, int crane_model)
{
	int first;
	struct line_s* stacks;
	struct line_s l;
	int stacks_count;
	FILE* f;
	int err;
	int i;
	int scanned;
	int move_count;
	int move_from;
	int move_to;
	int closed;

	assert(input);
	assert(input[0]);

	first = 1;
	stacks = NULL;
	mk_aoc2022_day05_line_construct(&l);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(f, &l);
		if(err != 0) return err;
		if(mk_aoc2022_day05_line_is_numbers(&l))
		{
			break;
		}
		err = mk_aoc2022_day05_line_extract_crates(&l);
		if(err != 0) return err;
		if(first)
		{
			first = 0;
			stacks_count = l.m_count;
			stacks = malloc(stacks_count * sizeof(struct line_s));
			if(!stacks) return ((int)(__LINE__));
			for(i = 0; i != stacks_count; ++i)
			{
				mk_aoc2022_day05_line_construct(&stacks[i]);
			}
		}
		if(l.m_count != stacks_count) return ((int)(__LINE__));
		for(i = 0; i != stacks_count; ++i)
		{
			if(l.m_elements[i] != ' ')
			{
				mk_aoc2022_day05_line_append(&stacks[i], l.m_elements[i]);
			}
		}
	}
	for(i = 0; i != stacks_count; ++i)
	{
		mk_aoc2022_day05_line_reverse(&stacks[i]);
	}
	err = mk_aoc2022_day05_line_read(f, &l);
	if(err != 0) return err;
	if(l.m_count != 0) return ((int)(__LINE__));
	for(;;)
	{
		scanned = fscanf(f, "move %d from %d to %d\x0a", &move_count, &move_from, &move_to);
		if(scanned != 3) break;
		if(crane_model == 9000)
		{
			for(i = 0; i != move_count; ++i)
			{
				mk_aoc2022_day05_line_append(&stacks[move_to - 1], stacks[move_from - 1].m_elements[stacks[move_from - 1].m_count - 1]);
				--stacks[move_from - 1].m_count;
			}
		}
		else if(crane_model == 9001)
		{
			for(i = 0; i != move_count; ++i)
			{
				mk_aoc2022_day05_line_append(&stacks[move_to - 1], stacks[move_from - 1].m_elements[stacks[move_from - 1].m_count - 1 - (move_count - 1) + i]);
			}
			stacks[move_from - 1].m_count -= move_count;
		}
		else
		{
			return ((int)(__LINE__));
		}
	}
	for(i = 0; i != stacks_count; ++i)
	{
		printf("%.1s", &stacks[i].m_elements[stacks[i].m_count - 1]);
	}
	printf("\n");
	for(i = 0; i != stacks_count; ++i)
	{
		mk_aoc2022_day05_line_destruct(&stacks[i]);
	}
	free(stacks);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&l);
	return 0;
}

int mk_aoc2022_day05a(char const* const input)
{
	return mk_aoc2022_day05(input, 9000);
}

int mk_aoc2022_day05b(char const* const input)
{
	return mk_aoc2022_day05(input, 9001);
}


int mk_aoc2022_day06(char const* const input, int run_len, int* out)
{
	int ret;
	struct line_s l;
	FILE* f;
	int err;
	int i;
	int j;
	int k;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	ret = 0;
	mk_aoc2022_day05_line_construct(&l);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	err = mk_aoc2022_day05_line_read(f, &l);
	if(err != 0) return err;
	if(!(l.m_count >= run_len)) return ((int)(__LINE__));
	i = 0;
	while(i <= l.m_count - run_len)
	{
		for(j = 0; j != run_len; ++j)
		{
			for(k = 0; k != run_len - 1 - j; ++k)
			{
				if(l.m_elements[i + run_len - 1 - j] == l.m_elements[i + run_len - 1 - j - 1 - k])
				{
					i += run_len - 1 - j - k;
					goto continue2;
				}
			}
		}
		ret = i + run_len;
		break;
		continue2:;
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&l);
	*out = ret;
	return 0;
}

int mk_aoc2022_day06a(char const* const input, int* out)
{
	return mk_aoc2022_day06(input, 4, out);
}

int mk_aoc2022_day06b(char const* const input, int* out)
{
	return mk_aoc2022_day06(input, 14, out);
}


struct tree_nodes_s;
struct tree_node_s;

void tree_nodes_construct(struct tree_nodes_s* tn);
void tree_nodes_destruct(struct tree_nodes_s* tn);
struct tree_node_s* tree_nodes_add_child(struct tree_nodes_s* tn, struct line_s* line);
struct tree_node_s* tree_nodes_find_child(struct tree_nodes_s* tn, char* name, int name_len);

void tree_node_construct(struct tree_node_s* tn);
void tree_node_destruct(struct tree_node_s* tn);
struct tree_node_s* tree_node_add_child(struct tree_node_s* tn, struct line_s* line);
struct tree_node_s* tree_node_find_child(struct tree_node_s* tn, char* name, int name_len);

struct tree_nodes_s
{
	int m_count;
	int m_capacity;
	struct tree_node_s** m_nodes;
};

struct tree_node_s
{
	struct line_s m_name_holder;
	int m_is_directory;
	int m_bytes;
	struct tree_node_s* m_parent;
	struct tree_nodes_s m_children;
};

void tree_nodes_construct(struct tree_nodes_s* tn)
{
	assert(tn);

	tn->m_count = 0;
	tn->m_capacity = 0;
	tn->m_nodes = NULL;
}

void tree_nodes_destruct(struct tree_nodes_s* tn)
{
	int i;

	assert(tn);

	for(i = 0; i != tn->m_count; ++i)
	{
		tree_node_destruct(tn->m_nodes[i]);
		free(tn->m_nodes[i]);
	}
	free(tn->m_nodes);
}

struct tree_node_s* tree_nodes_add_child(struct tree_nodes_s* tn, struct line_s* line)
{
	struct tree_node_s** nodes;
	struct tree_node_s* node;

	assert(tn);
	assert(line);

	if(tn->m_nodes == NULL || tn->m_count == tn->m_capacity)
	{
		if(tn->m_capacity == 0) tn->m_capacity = 4;
		tn->m_capacity *= 2;
		nodes = malloc(tn->m_capacity * sizeof(*nodes));
		if(nodes == NULL) crash();
		memcpy(nodes, tn->m_nodes, tn->m_count * sizeof(*nodes));
		free(tn->m_nodes);
		tn->m_nodes = nodes;
	}
	node = malloc(sizeof(*node));
	if(node == NULL) crash();
	tree_node_construct(node);
	node->m_name_holder = *line;
	mk_aoc2022_day05_line_construct(line);
	tn->m_nodes[tn->m_count] = node;
	++tn->m_count;
	return node;
}

struct tree_node_s* tree_nodes_find_child(struct tree_nodes_s* tn, char* name, int name_len)
{
	int i;
	struct tree_node_s* node;

	for(i = 0; i != tn->m_count; ++i)
	{
		node = tn->m_nodes[i];
		if
		(
			(node->m_is_directory == 1) &&
			(node->m_name_holder.m_count - 4 == name_len) &&
			(memcmp(node->m_name_holder.m_elements + 4, name, name_len) == 0)
		)
		{
			return node;
		}
	}
	crash();
	return NULL;
}

void tree_node_construct(struct tree_node_s* tn)
{
	assert(tn);

	mk_aoc2022_day05_line_construct(&tn->m_name_holder);
	tn->m_is_directory = 0;
	tn->m_bytes = 0;
	tree_nodes_construct(&tn->m_children);
	tn->m_parent = NULL;
}

void tree_node_destruct(struct tree_node_s* tn)
{
	assert(tn);

	mk_aoc2022_day05_line_destruct(&tn->m_name_holder);
	tree_nodes_destruct(&tn->m_children);
}

struct tree_node_s* tree_node_add_child(struct tree_node_s* tn, struct line_s* line)
{
	struct tree_node_s* child;

	assert(tn);
	assert(line);

	child = tree_nodes_add_child(&tn->m_children, line);
	child->m_parent = tn;
	return child;
}

struct tree_node_s* tree_node_find_child(struct tree_node_s* tn, char* name, int name_len)
{
	return tree_nodes_find_child(&tn->m_children, name, name_len);
}

void tree_sum_small_dirs_r(struct tree_node_s* tn, int limit, int* sum)
{
	int i;
	struct tree_node_s* node;

	tn->m_bytes = 0;
	for(i = 0; i != tn->m_children.m_count; ++i)
	{
		node = tn->m_children.m_nodes[i];
		if(node->m_is_directory)
		{
			tree_sum_small_dirs_r(node, limit, sum);
		}
		tn->m_bytes += node->m_bytes;
	}
	if(tn->m_bytes < limit)
	{
		*sum += tn->m_bytes;
	}
}

int tree_sum_small_dirs(struct tree_node_s* tn, int limit)
{
	int sum;

	sum = 0;
	tree_sum_small_dirs_r(tn, limit, &sum);
	return sum;
}

void tree_find_dir_to_delete_r(struct tree_node_s* tn, int needs_to_delete_at_least, struct tree_node_s** smallest_big_enough)
{
	int i;
	struct tree_node_s* node;

	if(tn->m_bytes >= needs_to_delete_at_least && tn->m_bytes < (*smallest_big_enough)->m_bytes)
	{
		*smallest_big_enough = tn;
	}
	for(i = 0; i != tn->m_children.m_count; ++i)
	{
		node = tn->m_children.m_nodes[i];
		if(node->m_is_directory)
		{
			tree_find_dir_to_delete_r(node, needs_to_delete_at_least, smallest_big_enough);
		}
	}
}

int tree_find_dir_to_delete(struct tree_node_s* tn, int fs_capacity, int update_size)
{
	struct tree_node_s* smallest_big_enough;
	int free_space;
	int needs_to_delete_at_least;

	tree_sum_small_dirs(tn, 0);
	smallest_big_enough = tn;
	free_space = fs_capacity - tn->m_bytes;
	needs_to_delete_at_least = update_size - free_space;
	tree_find_dir_to_delete_r(tn, needs_to_delete_at_least, &smallest_big_enough);
	return smallest_big_enough->m_bytes;
}

int mk_aoc2022_day07(char const* const input, int operation, int* out)
{
	static char const s_command_start[] = "$ ";
	static char const s_command_cd_root[] = "$ cd /";
	static char const s_command_cd_start[] = "$ cd ";
	static char const s_command_ls[] = "$ ls";
	static char const s_dir_begin[] = "dir ";

	enum expecting_e
	{
		expecting_command,
		expecting_ls_output
	};

	int ret;
	struct line_s l;
	FILE* f;
	int err;
	struct tree_node_s root;
	struct tree_node_s* current;
	enum expecting_e expecting;
	char* folder_name;
	int folder_name_len;
	struct tree_node_s* tmp;
	int scanned;
	int bytes;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	ret = 0;
	mk_aoc2022_day05_line_construct(&l);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	err = mk_aoc2022_day05_line_read(f, &l);
	if(err != 0) return err;
	if(l.m_count == 0) return ((int)(__LINE__));
	if(!(l.m_count == sizeof(s_command_cd_root) / sizeof(s_command_cd_root[0]) - 1)) return ((int)(__LINE__));
	if(memcmp(l.m_elements, s_command_cd_root, sizeof(s_command_cd_root) / sizeof(s_command_cd_root[0]) - 1) != 0) return ((int)(__LINE__));
	tree_node_construct(&root);
	current = &root;
	expecting = expecting_command;
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(f, &l);
		if(err != 0) return err;
		if(l.m_count == 0) break;
		switch(expecting)
		{
			case expecting_command:
			{
				if(!(l.m_count >= sizeof(s_command_start) / sizeof(s_command_start[0]) - 1 + 1)) return ((int)(__LINE__));
				if(memcmp(l.m_elements, s_command_start, sizeof(s_command_start) / sizeof(s_command_start[0]) - 1) != 0) return ((int)(__LINE__));
				gcommand:;
				if
				(
					(l.m_count == sizeof(s_command_ls) / sizeof(s_command_ls[0]) - 1) &&
					(memcmp(l.m_elements, s_command_ls, sizeof(s_command_ls) / sizeof(s_command_ls[0]) - 1) == 0)
				)
				{
					expecting = expecting_ls_output;
				}
				else if
				(
					(l.m_count > sizeof(s_command_cd_start) / sizeof(*s_command_cd_start) - 1) &&
					(memcmp(l.m_elements, s_command_cd_start, sizeof(s_command_cd_start) / sizeof(*s_command_cd_start) - 1) == 0)
				)
				{
					folder_name = l.m_elements + (sizeof(s_command_cd_start) / sizeof(*s_command_cd_start) - 1);
					folder_name_len = l.m_count - (sizeof(s_command_cd_start) / sizeof(*s_command_cd_start) - 1);
					if(folder_name_len == 2 && folder_name[0] == '.' && folder_name[1] == '.')
					{
						current = current->m_parent;
					}
					else
					{
						current = tree_node_find_child(current, folder_name, folder_name_len);
					}
				}
				else
				{
					return ((int)(__LINE__));
				}
			}
			break;
			case expecting_ls_output:
			{
				if
				(
					(l.m_count >= sizeof(s_command_start) / sizeof(s_command_start[0]) - 1 + 1) &&
					(memcmp(l.m_elements, s_command_start, sizeof(s_command_start) / sizeof(s_command_start[0]) - 1) == 0)
				)
				{
					expecting = expecting_command;
					goto gcommand;
				}
				if
				(
					(l.m_count > sizeof(s_dir_begin) / sizeof(s_dir_begin[0]) - 1) &&
					(memcmp(l.m_elements, s_dir_begin, sizeof(s_dir_begin) / sizeof(s_dir_begin[0]) - 1) == 0)
				)
				{
					tmp = tree_node_add_child(current, &l);
					tmp->m_is_directory = 1;
				}
				else
				{
					scanned = sscanf(l.m_elements, "%d ", &bytes);
					if(scanned != 1) return ((int)(__LINE__));
					tmp = tree_node_add_child(current, &l);
					tmp->m_bytes = bytes;
				}
			}
			break;
		}
	}
	if(operation == 0)
	{
		ret = tree_sum_small_dirs(&root, 100000);
	}
	else
	{
		ret = tree_find_dir_to_delete(&root, 70000000, 30000000);
	}
	tree_node_destruct(&root);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&l);
	*out = ret;
	return 0;
}

int mk_aoc2022_day07a(char const* const input, int* out)
{
	return mk_aoc2022_day07(input, 0, out);
}

int mk_aoc2022_day07b(char const* const input, int* out)
{
	return mk_aoc2022_day07(input, 1, out);
}


struct lines_s
{
	int m_count;
	int m_capacity;
	struct line_s* m_elements;
};

void lines_construct(struct lines_s* ls)
{
	assert(ls);

	ls->m_count = 0;
	ls->m_capacity = 0;
	ls->m_elements = NULL;
}

void lines_destruct(struct lines_s* ls)
{
	int i;

	assert(ls);

	for(i = 0; i != ls->m_count; ++i)
	{
		mk_aoc2022_day05_line_destruct(&ls->m_elements[i]);
	}
	free(ls->m_elements);
}

void lines_add(struct lines_s* ls, struct line_s* l)
{
	struct line_s* new_elements;

	assert(ls);

	if(ls->m_count == ls->m_capacity)
	{
		ls->m_capacity *= 2;
		if(ls->m_capacity == 0) ls->m_capacity = 8;
		new_elements = malloc(ls->m_capacity * sizeof(*new_elements));
		if(new_elements == NULL) crash();
		memcpy(new_elements, ls->m_elements, ls->m_count * sizeof(*new_elements));
		free(ls->m_elements);
		ls->m_elements = new_elements;
	}
	ls->m_elements[ls->m_count] = *l;
	++ls->m_count;
	mk_aoc2022_day05_line_construct(l);
}

int forest_algo_a(struct lines_s* lines)
{
	int height;
	int width;
	int i;
	int j;
	int ret;
	int tall;
	int visible;
	int k;

	assert(lines);
	assert(lines->m_count != 0);

	height = lines->m_count;
	width = lines->m_elements[0].m_count;
	if(!(height >= 3 && height >= 3)) crash();
	for(i = 0; i != height; ++i)
	{
		for(j = 0; j != width; ++j)
		{
			if(!(lines->m_elements[i].m_elements[j] >= '0' && lines->m_elements[i].m_elements[j] <= '9')) crash();
		}
	}
	ret = width * 2 + height * 2 - 4;
	for(i = 1; i != height - 1; ++i)
	{
		for(j = 1; j != width - 1; ++j)
		{
			tall = (lines->m_elements[i].m_elements[j] - '0');
			visible = 1;
			for(k = 0; k != i; ++k)
			{
				if((lines->m_elements[i - k - 1].m_elements[j] - '0') >= tall)
				{
					visible = 0;
					break;
				}
			}
			if(visible == 1)
			{
				++ret;
				continue;
			}
			visible = 1;
			for(k = 0; k != height - i - 1; ++k)
			{
				if((lines->m_elements[i + k + 1].m_elements[j] - '0') >= tall)
				{
					visible = 0;
					break;
				}
			}
			if(visible == 1)
			{
				++ret;
				continue;
			}
			visible = 1;
			for(k = 0; k != j; ++k)
			{
				if((lines->m_elements[i].m_elements[j - k - 1] - '0') >= tall)
				{
					visible = 0;
					break;
				}
			}
			if(visible == 1)
			{
				++ret;
				continue;
			}
			visible = 1;
			for(k = 0; k != width - j - 1; ++k)
			{
				if((lines->m_elements[i].m_elements[j + k + 1] - '0') >= tall)
				{
					visible = 0;
					break;
				}
			}
			if(visible == 1)
			{
				++ret;
				continue;
			}
		}
	}
	return ret;
}

int forest_tree_score(struct lines_s* lines, int h, int w)
{
	int height;
	int width;
	int tall;
	int count;
	int i;
	int total;

	height = lines->m_count;
	width = lines->m_elements[0].m_count;
	tall = lines->m_elements[h].m_elements[w];
	count = 0;
	for(i = 0; i != h; ++i)
	{
		++count;
		if(lines->m_elements[h - 1 - i].m_elements[w] >= tall) break;
	}
	total = count;
	count = 0;
	for(i = 0; i != height - 1 - h; ++i)
	{
		++count;
		if(lines->m_elements[h + 1 + i].m_elements[w] >= tall) break;
	}
	total *= count;
	count = 0;
	for(i = 0; i != w; ++i)
	{
		++count;
		if(lines->m_elements[h].m_elements[w - 1 - i] >= tall) break;
	}
	total *= count;
	count = 0;
	for(i = 0; i != width - 1 - w; ++i)
	{
		++count;
		if(lines->m_elements[h].m_elements[w + 1 + i] >= tall) break;
	}
	total *= count;
	return total;
}

int forest_algo_b(struct lines_s* lines)
{
	int height;
	int width;
	int i;
	int j;
	int best_score;
	int score;

	assert(lines);
	assert(lines->m_count != 0);

	height = lines->m_count;
	width = lines->m_elements[0].m_count;
	if(!(height >= 3 && height >= 3)) crash();
	for(i = 0; i != height; ++i)
	{
		for(j = 0; j != width; ++j)
		{
			if(!(lines->m_elements[i].m_elements[j] >= '0' && lines->m_elements[i].m_elements[j] <= '9')) crash();
		}
	}
	best_score = 0;
	for(i = 0; i != height; ++i)
	{
		for(j = 0; j != width; ++j)
		{
			score = forest_tree_score(lines, i, j);
			if(score > best_score) best_score = score;
		}
	}
	return best_score;
}

int mk_aoc2022_day08(char const* const input, int algo, int* out)
{
	int ret;
	struct lines_s lines;
	struct line_s line;
	FILE* f;
	int err;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	ret = 0;
	lines_construct(&lines);
	mk_aoc2022_day05_line_construct(&line);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(line.m_count == 0) return ((int)(__LINE__));
	lines_add(&lines, &line);
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(f, &line);
		if(err != 0) return err;
		if(line.m_count == 0) break;
		if(line.m_count != lines.m_elements[0].m_count) return ((int)(__LINE__));
		lines_add(&lines, &line);
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&line);
	if(algo == 0)
	{
		ret = forest_algo_a(&lines);
	}
	else
	{
		ret = forest_algo_b(&lines);
	}
	lines_destruct(&lines);
	*out = ret;
	return 0;
}

int mk_aoc2022_day08a(char const* const input, int* out)
{
	return mk_aoc2022_day08(input, 0, out);
}

int mk_aoc2022_day08b(char const* const input, int* out)
{
	return mk_aoc2022_day08(input, 1, out);
}


struct set_s
{
	int m_count;
	int m_capacity;
	unsigned* m_elements;
};

void set_construct(struct set_s* s)
{
	assert(s);

	s->m_count = 0;
	s->m_capacity = 0;
	s->m_elements = NULL;
}

void set_destruct(struct set_s* s)
{
	assert(s);

	free(s->m_elements);
}

void set_insert(struct set_s* s, unsigned item)
{
	int lower;
	int upper;
	int pos;
	unsigned* nelements;

	assert(s);

	lower = 0;
	upper = s->m_count;
	pos = (lower + upper) / 2;
	while(lower != upper)
	{
		if(s->m_elements[pos] == item)
		{
			return;
		}
		else if(s->m_elements[pos] < item)
		{
			lower = pos + 1;
		}
		else
		{
			upper = pos;
		}
		pos = (lower + upper) / 2;
	}
	if(s->m_count == s->m_capacity)
	{
		s->m_capacity *= 2;
		if(s->m_capacity == 0) s->m_capacity = 8;
		nelements = malloc(s->m_capacity * sizeof(*nelements));
		if(nelements == NULL) crash();
		memcpy(nelements, s->m_elements, s->m_count * sizeof(*nelements));
		free(s->m_elements);
		s->m_elements = nelements;
	}
	memmove(s->m_elements + pos + 1, s->m_elements + pos, (s->m_count - pos) * sizeof(*nelements));
	s->m_elements[pos] = item;
	++s->m_count;
}

int mk_aoc2022_day09a(char const* const input, int* out)
{
	int ret;
	struct line_s line;
	FILE* f;
	struct set_s set;
	int head_x;
	int head_y;
	int tail_x;
	int tail_y;
	unsigned pos;
	int err;
	int scanned;
	int n;
	int i;
	int distance_x;
	int distance_y;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	ret = 0;
	mk_aoc2022_day05_line_construct(&line);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	set_construct(&set);
	head_x = 0;
	head_y = 0;
	tail_x = head_x;
	tail_y = head_y;
	pos = ((unsigned)(((unsigned)(((unsigned)(((unsigned short)(tail_x)))) << 16)) | ((unsigned)(((unsigned short)(tail_y))))));
	set_insert(&set, pos);
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(f, &line);
		if(err != 0) return err;
		if(line.m_count == 0) break;
		scanned = sscanf(line.m_elements + 2, "%d", &n);
		if(scanned != 1) crash();
		for(i = 0; i != n; ++i)
		{
			switch(line.m_elements[0])
			{
				case 'U':
				{
					++head_y;
				}
				break;
				case 'R':
				{
					++head_x;
				}
				break;
				case 'D':
				{
					--head_y;
				}
				break;
				case 'L':
				{
					--head_x;
				}
				break;
			}
			distance_x = head_x - tail_x;
			distance_y = head_y - tail_y;
			if(distance_x == +2) ++tail_x;
			if(distance_x == -2) --tail_x;
			if(distance_y == +2) ++tail_y;
			if(distance_y == -2) --tail_y;
			if((distance_x == 2 || distance_x == -2) && distance_y != 0) tail_y += distance_y;
			if((distance_y == 2 || distance_y == -2) && distance_x != 0) tail_x += distance_x;
			pos = ((unsigned)(((unsigned)(((unsigned)(((unsigned short)(tail_x)))) << 16)) | ((unsigned)(((unsigned short)(tail_y))))));
			set_insert(&set, pos);
		}
	}
	ret = set.m_count;
	set_destruct(&set);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&line);
	*out = ret;
	return 0;
}

void update_tail(int head_x, int head_y, int* tail_x, int* tail_y)
{
	int distance_x;
	int distance_y;

	distance_x = head_x - *tail_x;
	distance_y = head_y - *tail_y;
	if(distance_x == +2){ ++*tail_x; }
	if(distance_x == -2){ --*tail_x; }
	if(distance_y == +2){ ++*tail_y; }
	if(distance_y == -2){ --*tail_y; }
	if((distance_x == 2 || distance_x == -2) && (distance_y == 1 || distance_y == -1)){ *tail_y += distance_y; }
	if((distance_y == 2 || distance_y == -2) && (distance_x == 1 || distance_x == -1)){ *tail_x += distance_x; }
}

int mk_aoc2022_day09b(char const* const input, int* out)
{
	enum
	{
		tail_count = 9
	};

	int ret;
	struct line_s line;
	FILE* f;
	struct set_s set;
	int head_x;
	int head_y;
	int it;
	int tail_x[tail_count];
	int tail_y[tail_count];
	unsigned pos;
	int err;
	int scanned;
	int n;
	int i;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	ret = 0;
	mk_aoc2022_day05_line_construct(&line);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	set_construct(&set);
	head_x = 0;
	head_y = 0;
	for(it = 0; it != tail_count; ++it)
	{
		tail_x[it] = head_x;
		tail_y[it] = head_y;
	}
	pos = ((unsigned)(((unsigned)(((unsigned)(((unsigned short)(((short)(tail_x[tail_count - 1])))))) << 16)) | ((unsigned)(((unsigned short)(((short)(tail_y[tail_count - 1]))))))));
	set_insert(&set, pos);
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(f, &line);
		if(err != 0) return err;
		if(line.m_count == 0) break;
		scanned = sscanf(line.m_elements + 2, "%d", &n);
		if(scanned != 1) crash();
		for(i = 0; i != n; ++i)
		{
			switch(line.m_elements[0])
			{
				case 'U':
				{
					++head_y;
				}
				break;
				case 'R':
				{
					++head_x;
				}
				break;
				case 'D':
				{
					--head_y;
				}
				break;
				case 'L':
				{
					--head_x;
				}
				break;
				default:
					crash();
				break;
			}
			update_tail(head_x, head_y, &tail_x[0], &tail_y[0]);
			for(it = 0; it != tail_count - 1; ++it)
			{
				update_tail(tail_x[it], tail_y[it], &tail_x[it + 1], &tail_y[it + 1]);
			}
			pos = ((unsigned)(((unsigned)(((unsigned)(((unsigned short)(((short)(tail_x[tail_count - 1])))))) << 16)) | ((unsigned)(((unsigned short)(((short)(tail_y[tail_count - 1]))))))));
			set_insert(&set, pos);
		}
	}
	ret = set.m_count;
	set_destruct(&set);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&line);
	*out = ret;
	return 0;
}


struct instruction_addx_s
{
	int m_param;
};

struct instruction_noop_s
{
	int m_dummy;
};

union instruction_u
{
	struct instruction_addx_s m_addx;
	struct instruction_noop_s m_noop;
};

enum instruction_e
{
	addx,
	noop
};

struct instruction_s
{
	enum instruction_e m_type;
	union instruction_u m_instr;
};

struct program_s
{
	int m_count;
	int m_capacity;
	struct instruction_s* m_instrs;
};

struct cpu_s
{
	int m_register_x;
};

struct computer_s
{
	struct cpu_s m_cpu;
	struct program_s m_program;
	int m_pc;
	int m_micro_step;
	int m_ticks;
};

void program_construct(struct program_s* program)
{
	assert(program);

	program->m_count = 0;
	program->m_capacity = 0;
	program->m_instrs = NULL;
}

void program_destruct(struct program_s* program)
{
	assert(program);

	free(program->m_instrs);
}

void program_append(struct program_s* program, struct instruction_s* instr)
{
	struct instruction_s* ninstrs;

	assert(program);
	assert(instr);

	if(program->m_count == program->m_capacity)
	{
		program->m_capacity *= 2;
		if(program->m_capacity == 0) program->m_capacity = 8;
		ninstrs = malloc(program->m_capacity * sizeof(*ninstrs));
		if(ninstrs == NULL) crash();
		memcpy(ninstrs, program->m_instrs, program->m_count * sizeof(*ninstrs));
		free(program->m_instrs);
		program->m_instrs = ninstrs;
	}
	program->m_instrs[program->m_count] = *instr;
	++program->m_count;
}

void computer_construct(struct computer_s* c)
{
	assert(c);

	c->m_cpu.m_register_x = 1;
	program_construct(&c->m_program);
	c->m_pc = 0;
	c->m_micro_step = 0;
	c->m_ticks = 0;
}

void computer_destruct(struct computer_s* c)
{
	assert(c);

	program_destruct(&c->m_program);
}

void computer_tick(struct computer_s* c)
{
	assert(c);

	++c->m_ticks;
	switch(c->m_program.m_instrs[c->m_pc].m_type)
	{
		case addx:
		{
			switch(c->m_micro_step)
			{
				case 0:
				{
					++c->m_micro_step;
				}
				break;
				case 1:
				{
					c->m_cpu.m_register_x += c->m_program.m_instrs[c->m_pc].m_instr.m_addx.m_param;
					c->m_micro_step = 0;
					++c->m_pc;
				}
				break;
				default:
				{
					assert(0);
				}
				break;
			}
		}
		break;
		case noop:
		{
			assert(c->m_micro_step == 0);
			++c->m_pc;
		}
		break;
		default:
		{
			assert(0);
		}
		break;
	}
}

struct instruction_s parse_instruction(struct line_s* line)
{
	static char const s_noop[] = "noop";
	static char const s_addx[] = "addx ";

	struct instruction_s instr;
	int scanned;

	assert(line);

	if((line->m_count == (sizeof(s_noop) / sizeof(*s_noop) - 1)) && (memcmp(line->m_elements, s_noop, (sizeof(s_noop) / sizeof(*s_noop) - 1)) == 0))
	{
		instr.m_type = noop;
	}
	else if((line->m_count > (sizeof(s_addx) / sizeof(*s_addx) - 1)) && (memcmp(line->m_elements, s_addx, (sizeof(s_addx) / sizeof(*s_addx) - 1)) == 0))
	{
		instr.m_type = addx;
		scanned = sscanf(line->m_elements + (sizeof(s_addx) / sizeof(*s_addx) - 1), "%d", &instr.m_instr.m_addx.m_param);
		if(scanned != 1) crash();
	}
	return instr;
}

int mk_aoc2022_day10(char const* const input, int variant, int* out)
{
	struct computer_s c;
	int ret;
	struct line_s line;
	FILE* f;
	int err;
	struct instruction_s instr;
	int i;
	int j;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	computer_construct(&c);
	ret = 0;
	mk_aoc2022_day05_line_construct(&line);
	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(f, &line);
		if(err != 0) return err;
		if(line.m_count == 0) break;
		instr = parse_instruction(&line);
		program_append(&c.m_program, &instr);
	}
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	mk_aoc2022_day05_line_destruct(&line);
	if(variant == 0)
	{
		++c.m_ticks;
		for(i = 0; i != 20-1; ++i)
		{
			computer_tick(&c);
		}
		ret = c.m_ticks * c.m_cpu.m_register_x;
		for(j = 0; j != 5; ++j)
		{
			for(i = 0; i != 40; ++i)
			{
				computer_tick(&c);
			}
			ret += c.m_ticks * c.m_cpu.m_register_x;
		}
	}
	else
	{
		for(j = 0; j != 6; ++j)
		{
			for(i = 0; i != 40; ++i)
			{
				if(i >= c.m_cpu.m_register_x - 1 && i <= c.m_cpu.m_register_x + 1)
				{
					printf("#");
				}
				else
				{
					printf(" ");
				}
				computer_tick(&c);
			}
			printf("\n");
		}
	}
	printf("\n");
	computer_destruct(&c);
	*out = ret;
	return 0;
}

int mk_aoc2022_day10a(char const* const input, int* out)
{
	return mk_aoc2022_day10(input, 0, out);
}

int mk_aoc2022_day10b(char const* const input, int* out)
{
	return mk_aoc2022_day10(input, 1, out);
}


struct ints_s
{
	int m_count;
	int m_capacity;
	int* m_elements;
};

void ints_construct(struct ints_s* ints)
{
	assert(ints);

	ints->m_count = 0;
	ints->m_capacity = 0;
	ints->m_elements = NULL;
}

void ints_destruct(struct ints_s* ints)
{
	assert(ints);

	free(ints->m_elements);
}

void ints_append(struct ints_s* ints, int x)
{
	int* nelems;

	assert(ints);

	if(ints->m_count == ints->m_capacity)
	{
		ints->m_capacity *= 2;
		if(ints->m_capacity == 0) ints->m_capacity = 8;
		nelems = malloc(ints->m_capacity * sizeof(*nelems));
		if(!nelems) crash();
		memcpy(nelems, ints->m_elements, ints->m_count * sizeof(*nelems));
		free(ints->m_elements);
		ints->m_elements = nelems;
	}
	ints->m_elements[ints->m_count] = x;
	++ints->m_count;
}

void ints_reverse(struct ints_s* ints)
{
	int i;
	int tmp;

	assert(ints);

	for(i = 0; i != ints->m_count / 2; ++i)
	{
		tmp = ints->m_elements[i];
		ints->m_elements[i] = ints->m_elements[ints->m_count - 1 - i];
		ints->m_elements[ints->m_count - 1 - i] = tmp;
	}
}


void ints_pop(struct ints_s* ints)
{
	assert(ints);
	assert(ints->m_count != 0);

	--ints->m_count;
}

enum math_operation_e
{
	math_add,
	math_sub,
	math_mul,
	math_div
};

#define math_use_old (-999999)

struct monkey_s
{
	struct ints_s m_items;
	enum math_operation_e m_worry_math;
	int m_worry_constant;
	int m_throw_logic_param;
	int m_true_idx;
	int m_false_idx;
	int m_curiosity;
};

void monkey_construct(struct monkey_s* monkey)
{
	assert(monkey);

	ints_construct(&monkey->m_items);
	monkey->m_curiosity = 0;
}

void monkey_destruct(struct monkey_s* monkey)
{
	assert(monkey);

	ints_destruct(&monkey->m_items);
}

struct monkeys_s
{
	int m_count;
	int m_capacity;
	struct monkey_s* m_elements;
};

void monkeys_construct(struct monkeys_s* monkeys)
{
	assert(monkeys);

	monkeys->m_count = 0;
	monkeys->m_capacity = 0;
	monkeys->m_elements = NULL;
}

void monkeys_destruct(struct monkeys_s* monkeys)
{
	int i;

	assert(monkeys);

	for(i = 0; i != monkeys->m_count; ++i)
	{
		monkey_destruct(&monkeys->m_elements[i]);
	}
	free(monkeys->m_elements);
}

void monkeys_add(struct monkeys_s* monkeys, struct monkey_s* monkey)
{
	struct monkey_s* nelems;

	assert(monkeys);
	assert(monkey);

	if(monkeys->m_count == monkeys->m_capacity)
	{
		monkeys->m_capacity *= 2;
		if(monkeys->m_capacity == 0) monkeys->m_capacity = 8;
		nelems = malloc(monkeys->m_capacity * sizeof(*nelems));
		if(!nelems) crash();
		memcpy(nelems, monkeys->m_elements, monkeys->m_count * sizeof(*nelems));
		free(monkeys->m_elements);
		monkeys->m_elements = nelems;
	}
	monkeys->m_elements[monkeys->m_count] = *monkey;
	++monkeys->m_count;
}

void parse_int(char const* const start, char const* const end, int* const err, char const** const nend, signed int* const value)
{
	#define bool_t int

	#define s_false ((bool_t)(0))
	#define s_true ((bool_t)(1))
	#define s_err_no ((int)(0))
	#define s_err_invalid ((int)(1))
	#define s_err_overflow ((int)(2))
	#define s_minus ((char)('-'))
	#define s_border_pos_val ((unsigned int)(((unsigned int)(((signed int)(INT_MAX)))) / ((unsigned int)(10))))
	#define s_border_pos_dig ((unsigned int)(((unsigned int)(((signed int)(INT_MAX)))) % ((unsigned int)(10))))
	#define s_border_neg_val ((unsigned int)(((unsigned int)(((unsigned int)(((signed int)(-((signed int)(((signed int)(INT_MIN)) + ((signed int)(16)))))))) + ((unsigned int)(16)))) / ((unsigned int)(10))))
	#define s_border_neg_dig ((unsigned int)(((unsigned int)(((unsigned int)(((signed int)(-((signed int)(((signed int)(INT_MIN)) + ((signed int)(16)))))))) + ((unsigned int)(16)))) % ((unsigned int)(10))))
	#define s_neg_pos_diff ((unsigned int)(((signed int)(((signed int)(-((signed int)(((signed int)(INT_MIN)) + ((signed int)(16)))))) - ((signed int)(((signed int)(INT_MAX)) - ((signed int)(16))))))))
	
	static char const s_digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	char const* it;
	bool_t negative;
	bool_t overflow;
	unsigned int val;
	char ch;
	int i;
	signed int ret;

	assert(start);
	assert(end);
	assert(end >= start);
	assert(err);
	assert(nend);
	assert(value);
	assert(sizeof(s_digits) / sizeof(*s_digits) == 10);
	assert(((signed int)(-((signed int)(((signed int)(INT_MIN)) + ((signed int)(16)))))) >= ((signed int)(((signed int)(INT_MAX)) - ((signed int)(16)))));
	assert(((unsigned int)(((unsigned int)(UINT_MAX)) - ((unsigned int)(16)))) >= ((unsigned int)(((signed int)(-((signed int)(((signed int)(INT_MIN)) + ((signed int)(16)))))))));

	it = start;
	negative = s_false;
	if(it != end)
	{
		overflow = s_false;
		val = ((unsigned int)(0));
		if(*it == s_minus)
		{
			negative = s_true;
			++it;
		}
		for(; it != end; ++it)
		{
			ch = *it;
			for(i = 0; i != 10; ++i)
			{
				if(ch == s_digits[i])
				{
					break;
				}
			}
			if(i == 10)
			{
				break;
			}
			if(!overflow)
			{
				if(negative ? ((val < s_border_neg_val) || (val == s_border_neg_val && i <= s_border_neg_dig)) : ((val < s_border_pos_val) || (val == s_border_pos_val && i <= s_border_pos_dig)))
				{
					val = ((unsigned int)(((unsigned int)(val * ((unsigned int)(10)))) + ((unsigned int)(i))));
				}
				else
				{
					overflow = s_true;
				}
			}
		}
	}
	if((it - start) == (negative ? 1 : 0))
	{
		*err = s_err_invalid;
		*nend = start;
	}
	else if(overflow)
	{
		*err = s_err_overflow;
		*nend = it;
	}
	else
	{
		if(negative)
		{
			if(val <= s_neg_pos_diff)
			{
				ret = ((signed int)(-((signed int)(val))));
			}
			else
			{
				ret = ((signed int)(((signed int)(-((signed int)(((unsigned int)(val - s_neg_pos_diff)))))) - ((signed int)(s_neg_pos_diff))));
			}
		}
		else
		{
			ret = ((signed int)(val));
		}
		*err = s_err_no;
		*nend = it;
		*value = ret;
	}

	#undef bool_t
	#undef s_false
	#undef s_true
	#undef s_err_no
	#undef s_err_invalid
	#undef s_err_overflow
	#undef s_minus
	#undef s_border_pos_val
	#undef s_border_pos_dig
	#undef s_border_neg_val
	#undef s_border_neg_dig
	#undef s_neg_pos_diff
}

int parse_monkey(FILE* f, struct monkey_s* monkey)
{
	static char const s_monkey_header[] = "Monkey ";
	static char const s_monkey_footer[] = ":";
	static char const s_starting_items_header[] = "  Starting items: ";
	static char const s_starting_items_separator[] = ", ";
	static char const s_operation_header[] = "  Operation: new = old ";
	static char const s_operation_add[] = "+ ";
	static char const s_operation_mul[] = "* ";
	static char const s_throw_header[] = "  Test: divisible by ";
	static char const s_true_header[] = "    If true: throw to monkey ";
	static char const s_false_header[] = "    If false: throw to monkey ";

	int err;
	struct line_s line;
	char const* nend;
	int val;

	assert(f);
	assert(monkey);

	monkey_construct(monkey);

	mk_aoc2022_day05_line_construct(&line);

	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(!(memcmp(line.m_elements, s_monkey_header, sizeof(s_monkey_header) / sizeof(*s_monkey_header) - 1) == 0)) return ((int)(__LINE__));
	parse_int(line.m_elements + sizeof(s_monkey_header) / sizeof(*s_monkey_header) - 1, line.m_elements + line.m_count - (sizeof(s_monkey_footer) / sizeof(*s_monkey_footer) - 1), &err, &nend, &val);
	if(err != 0) return err;
	if(line.m_count - (nend - line.m_elements) != sizeof(s_monkey_footer) / sizeof(*s_monkey_footer) - 1) return ((int)(__LINE__));
	if(!(memcmp(nend, s_monkey_footer, sizeof(s_monkey_footer) / sizeof(*s_monkey_footer) - 1) == 0)) return ((int)(__LINE__));

	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(!(memcmp(line.m_elements, s_starting_items_header, sizeof(s_starting_items_header) / sizeof(*s_starting_items_header) - 1) == 0)) return ((int)(__LINE__));
	nend = line.m_elements + (sizeof(s_starting_items_header) / sizeof(*s_starting_items_header) - 1);
	parse_int(nend, line.m_elements + line.m_count, &err, &nend, &val);
	if(err != 0) return err;
	ints_append(&monkey->m_items, val);
	while(nend != line.m_elements + line.m_count)
	{
		if(!(memcmp(nend, s_starting_items_separator, sizeof(s_starting_items_separator) / sizeof(*s_starting_items_separator) - 1) == 0)) return ((int)(__LINE__));
		nend += (sizeof(s_starting_items_separator) / sizeof(*s_starting_items_separator) - 1);
		parse_int(nend, line.m_elements + line.m_count, &err, &nend, &val);
		if(err != 0) return err;
		ints_append(&monkey->m_items, val);
	}

	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(!(memcmp(line.m_elements, s_operation_header, sizeof(s_operation_header) / sizeof(*s_operation_header) - 1) == 0)) return ((int)(__LINE__));
	if(memcmp(line.m_elements + (sizeof(s_operation_header) / sizeof(*s_operation_header) - 1), s_operation_add, sizeof(s_operation_add) / sizeof(*s_operation_add) - 1) == 0)
	{
		monkey->m_worry_math = math_add;
	}
	else if(memcmp(line.m_elements + (sizeof(s_operation_header) / sizeof(*s_operation_header) - 1), s_operation_mul, sizeof(s_operation_mul) / sizeof(*s_operation_mul) - 1) == 0)
	{
		monkey->m_worry_math = math_mul;
	}
	else
	{
		return ((int)(__LINE__));
	}
	parse_int(line.m_elements + (sizeof(s_operation_header) / sizeof(*s_operation_header) - 1) + 2, line.m_elements + line.m_count, &err, &nend, &val);
	if(err == 0)
	{
		assert(val != math_use_old);
		monkey->m_worry_constant = val;
	}
	else
	{
		monkey->m_worry_constant = math_use_old;
	}

	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(!(memcmp(line.m_elements, s_throw_header, sizeof(s_throw_header) / sizeof(*s_throw_header) - 1) == 0)) return ((int)(__LINE__));
	parse_int(line.m_elements + (sizeof(s_throw_header) / sizeof(*s_throw_header) - 1), line.m_elements + line.m_count, &err, &nend, &val);
	if(err != 0) return err;
	monkey->m_throw_logic_param = val;

	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(!(memcmp(line.m_elements, s_true_header, sizeof(s_true_header) / sizeof(*s_true_header) - 1) == 0)) return ((int)(__LINE__));
	parse_int(line.m_elements + (sizeof(s_true_header) / sizeof(*s_true_header) - 1), line.m_elements + line.m_count, &err, &nend, &val);
	if(err != 0) return err;
	monkey->m_true_idx = val;

	err = mk_aoc2022_day05_line_read(f, &line);
	if(err != 0) return err;
	if(!(memcmp(line.m_elements, s_false_header, sizeof(s_false_header) / sizeof(*s_false_header) - 1) == 0)) return ((int)(__LINE__));
	parse_int(line.m_elements + (sizeof(s_false_header) / sizeof(*s_false_header) - 1), line.m_elements + line.m_count, &err, &nend, &val);
	if(err != 0) return err;
	monkey->m_false_idx = val;

	mk_aoc2022_day05_line_destruct(&line);

	return 0;
}

long long int monkeys_play_game(struct monkeys_s* monkeys, int rounds, int div)
{
	long long int all;
	int imonkey;
	struct monkey_s* monkey;
	int iround;
	long long int worry;
	int target_monkey;
	long long int curiosity[2];
	long long int ret;

	assert(monkeys);

	all = 1;
	for(imonkey = 0; imonkey != monkeys->m_count; ++imonkey)
	{
		monkey = &monkeys->m_elements[imonkey];
		all *= monkey->m_throw_logic_param;
	}
	for(iround = 0; iround != rounds; ++iround)
	{
		for(imonkey = 0; imonkey != monkeys->m_count; ++imonkey)
		{
			monkey = &monkeys->m_elements[imonkey];
			monkey->m_curiosity += monkey->m_items.m_count;
			ints_reverse(&monkey->m_items);
			while(monkey->m_items.m_count != 0)
			{
				worry = monkey->m_items.m_elements[monkey->m_items.m_count - 1];
				ints_pop(&monkey->m_items);
				if(monkey->m_worry_math == math_add)
				{
					assert(monkey->m_worry_constant != math_use_old);
					worry += monkey->m_worry_constant;
				}
				else if(monkey->m_worry_math == math_mul)
				{
					if(monkey->m_worry_constant == math_use_old)
					{
						worry *= worry;
					}
					else
					{
						worry *= monkey->m_worry_constant;
					}
				}
				worry /= div;
				worry %= all;
				target_monkey = ((worry % monkey->m_throw_logic_param) == 0) ? (monkey->m_true_idx) : (monkey->m_false_idx);
				ints_append(&monkeys->m_elements[target_monkey].m_items, ((int)(worry)));
			}
		}
	}
	curiosity[0] = 0;
	curiosity[1] = 0;
	for(imonkey = 0; imonkey != monkeys->m_count; ++imonkey)
	{
		monkey = &monkeys->m_elements[imonkey];
		if(monkey->m_curiosity > curiosity[0])
		{
			curiosity[1] = curiosity[0];
			curiosity[0] = monkey->m_curiosity;
		}
		else if(monkey->m_curiosity > curiosity[1])
		{
			curiosity[1] = monkey->m_curiosity;
		}
	}
	ret = curiosity[0] * curiosity[1];
	return ret;
}

int mk_aoc2022_day11(char const* const input, int rounds, int div, long long int* out)
{
	FILE* f;
	struct monkeys_s monkeys;
	struct monkey_s monkey;
	int err;
	struct line_s line;
	int closed;

	assert(input);
	assert(input[0]);
	assert(out);

	f = fopen(input, "rb");
	if(!f) return ((int)(__LINE__));
	monkeys_construct(&monkeys);
	mk_aoc2022_day05_line_construct(&line);
	for(;;)
	{
		err = parse_monkey(f, &monkey);
		if(err != 0) break;
		monkeys_add(&monkeys, &monkey);
		err = mk_aoc2022_day05_line_read(f, &line);
		if(err != 0) break;
	}
	mk_aoc2022_day05_line_destruct(&line);
	closed = fclose(f);
	if(closed != 0) return ((int)(__LINE__));
	*out = monkeys_play_game(&monkeys, rounds, div);
	monkeys_destruct(&monkeys);
	return 0;
}

int mk_aoc2022_day11a(char const* const input, long long int* out)
{
	return mk_aoc2022_day11(input, 20, 3, out);
}

int mk_aoc2022_day11b(char const* const input, long long int* out)
{
	return mk_aoc2022_day11(input, 10000, 1, out);
}


struct grid_s
{
	int m_width;
	int m_height;
	int* m_elements;
};

void grid_construct(struct grid_s* grid, int width, int height)
{
	assert(grid);

	grid->m_width = width;
	grid->m_height = height;
	grid->m_elements = malloc(grid->m_width * grid->m_height * sizeof(*grid->m_elements));
	if(!grid->m_elements) crash();
}

void grid_destruct(struct grid_s* grid)
{
	assert(grid);

	free(grid->m_elements);
}

int grid_get(struct grid_s* grid, int x, int y)
{
	assert(grid);
	assert(x >= 0 && x < grid->m_width);
	assert(y >= 0 && y < grid->m_height);

	return grid->m_elements[y * grid->m_width + x];
}

void grid_set(struct grid_s* grid, int x, int y, int val)
{
	assert(grid);
	assert(x >= 0 && x < grid->m_width);
	assert(y >= 0 && y < grid->m_height);

	grid->m_elements[y * grid->m_width + x] = val;
}

void grid_fill(struct grid_s* grid, int val)
{
	int y;
	int x;

	assert(grid);

	for(y = 0; y != grid->m_height; ++y)
	{
		for(x = 0; x != grid->m_width; ++x)
		{
			grid_set(grid, x, y, val);
		}
	}
}

struct day12_s
{
	struct grid_s m_grid;
	int m_startx;
	int m_starty;
	int m_goalx;
	int m_goaly;
};

void day12_construct(struct day12_s* day12, char const* const filename)
{
	FILE* file;
	struct line_s line;
	struct lines_s lines;
	int err;
	int y;
	int x;
	char ch;
	int closed;

	assert(day12);
	assert(filename);
	assert(*filename);

	file = fopen(filename, "rb");
	if(!file) crash();
	mk_aoc2022_day05_line_construct(&line);
	err = mk_aoc2022_day05_line_read(file, &line);
	if(err != 0) crash();
	lines_construct(&lines);
	lines_add(&lines, &line);
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count == 0) break;
		if(line.m_count != lines.m_elements[0].m_count) crash();
		lines_add(&lines, &line);
	}
	grid_construct(&day12->m_grid, lines.m_elements[0].m_count, lines.m_count);
	for(y = 0; y != day12->m_grid.m_height; ++y)
	{
		for(x = 0; x != day12->m_grid.m_width; ++x)
		{
			ch = lines.m_elements[y].m_elements[x];
			if(ch == 'S')
			{
				day12->m_startx = x;
				day12->m_starty = y;
				ch = 'a';
			}
			else if(ch == 'E')
			{
				day12->m_goalx = x;
				day12->m_goaly = y;
				ch = 'z';
			}
			grid_set(&day12->m_grid, x, y, ch - 'a');
		}
	}
	lines_destruct(&lines);
	mk_aoc2022_day05_line_destruct(&line);
	closed = fclose(file);
	if(closed != 0) crash();
}

void day12_destruct(struct day12_s* day12)
{
	assert(day12);

	grid_destruct(&day12->m_grid);
}

unsigned int_pack(int a, int b)
{
	assert(a >= 0 && a <= 0xffff);
	assert(b >= 0 && b <= 0xffff);

	return ((unsigned)(((unsigned)(((unsigned)(a)) << 16)) | ((unsigned)(b))));
}

void int_unpack(unsigned x, int* a, int* b)
{
	assert(a);
	assert(b);

	*a = ((int)(((unsigned)(x >> 16))));
	*b = ((int)(((unsigned)(x & 0xffff))));
}

int day12_find_path(struct day12_s* day12)
{
	int ret;
	struct grid_s cost;
	struct ints_s stack;
	int x;
	int y;
	int ce; /* current elevation */
	int cc; /* current cost */
	int te; /* target elevation */
	int tc; /* target cost */

	assert(day12);

	ret = 0;
	grid_construct(&cost, day12->m_grid.m_width, day12->m_grid.m_height);
	grid_fill(&cost, INT_MAX);
	grid_set(&cost, day12->m_startx, day12->m_starty, 0);
	ints_construct(&stack);
	ints_append(&stack, ((int)(int_pack(day12->m_startx, day12->m_starty))));
	while(stack.m_count != 0)
	{
		int_unpack(stack.m_elements[stack.m_count - 1], &x, &y);
		ints_pop(&stack);
		ce = grid_get(&day12->m_grid, x, y);
		cc = grid_get(&cost, x, y);
		if(y != 0)
		{
			te = grid_get(&day12->m_grid, x, y - 1);
			tc = grid_get(&cost, x, y - 1);
			if(ce + 1 >= te && tc > cc + 1)
			{
				grid_set(&cost, x, y - 1, cc + 1);
				ints_append(&stack, ((int)(int_pack(x, y - 1))));
			}
		}
		if(x != 0)
		{
			te = grid_get(&day12->m_grid, x - 1, y);
			tc = grid_get(&cost, x - 1, y);
			if(ce + 1 >= te && tc > cc + 1)
			{
				grid_set(&cost, x - 1, y, cc + 1);
				ints_append(&stack, ((int)(int_pack(x - 1, y))));
			}
		}
		if(y != cost.m_height - 1)
		{
			te = grid_get(&day12->m_grid, x, y + 1);
			tc = grid_get(&cost, x, y + 1);
			if(ce + 1 >= te && tc > cc + 1)
			{
				grid_set(&cost, x, y + 1, cc + 1);
				ints_append(&stack, ((int)(int_pack(x, y + 1))));
			}
		}
		if(x != cost.m_width - 1)
		{
			te = grid_get(&day12->m_grid, x + 1, y);
			tc = grid_get(&cost, x + 1, y);
			if(ce + 1 >= te && tc > cc + 1)
			{
				grid_set(&cost, x + 1, y, cc + 1);
				ints_append(&stack, ((int)(int_pack(x + 1, y))));
			}
		}
	}
	ret = grid_get(&cost, day12->m_goalx, day12->m_goaly);
	ints_destruct(&stack);
	grid_destruct(&cost);
	return ret;
}

int day12_find_path2(struct day12_s* day12)
{
	int ret;
	int x;
	int y;
	int cost;

	assert(day12);

	ret = INT_MAX;
	for(y = 0; y != day12->m_grid.m_height; ++y)
	{
		for(x = 0; x != day12->m_grid.m_width; ++x)
		{
			if(grid_get(&day12->m_grid, x, y) == 0)
			{
				day12->m_startx = x;
				day12->m_starty = y;
				cost = day12_find_path(day12);
				if(cost < ret)
				{
					ret = cost;
				}
			}
		}
	}
	return ret;
}

void mk_aoc2022_day12a(char const* const input, int* out)
{
	struct day12_s day12;

	assert(input);
	assert(out);

	day12_construct(&day12, input);
	*out = day12_find_path(&day12);
	day12_destruct(&day12);
}

void mk_aoc2022_day12b(char const* const input, int* out)
{
	struct day12_s day12;

	assert(input);
	assert(out);

	day12_construct(&day12, input);
	*out = day12_find_path2(&day12);
	day12_destruct(&day12);
}


struct list_element_s;

struct list_s
{
	int m_count;
	int m_capacity;
	struct list_element_s* m_elements;
};

union list_element_u
{
	int m_single;
	struct list_s m_multi;
};

struct list_element_s
{
	int m_type;
	union list_element_u m_data;
};

void list_construct(struct list_s* list)
{
	assert(list);

	list->m_count = 0;
	list->m_capacity = 0;
	list->m_elements = NULL;
}

void list_destruct(struct list_s* list)
{
	int n;
	int i;

	assert(list);

	n = list->m_count;;
	for(i = 0; i != n; ++i)
	{
		if(list->m_elements[i].m_type == 1)
		{
			list_destruct(&list->m_elements[i].m_data.m_multi);
		}
	}
	free(list->m_elements);
}

void list_add(struct list_s* list, struct list_element_s* item)
{
	struct list_element_s* nelements;

	assert(list);
	assert(item);

	if(list->m_count == list->m_capacity)
	{
		list->m_capacity *= 2;
		if(list->m_capacity == 0) list->m_capacity = 8;
		nelements = malloc(list->m_capacity * sizeof(*nelements));
		if(!nelements) crash();
		memcpy(nelements, list->m_elements, list->m_count * sizeof(*nelements));
		free(list->m_elements);
		list->m_elements = nelements;
	}
	list->m_elements[list->m_count] = *item;
	++list->m_count;
}

void list_parse_r(char const** ptr, struct list_s* list)
{
	char const* it;
	int err;
	char const* nend;
	int value;
	struct list_element_s item;

	assert(ptr);
	assert(*ptr);
	assert(list);

	it = *ptr;
	list_construct(list);
	if(*it != '[') crash();
	++it;
	for(;;)
	{
		if(*it == ']')
		{
			++it;
			if(*it == ',') ++it;
			break;
		}
		else if(*it == '[')
		{
			item.m_type = 1;
			list_parse_r(&it, &item.m_data.m_multi);
			list_add(list, &item);
			if(*it == ',') ++it;
		}
		else
		{
			parse_int(it, it + 999, &err, &nend, &value);
			if(err != 0) crash();
			item.m_type = 0;
			item.m_data.m_single = value;
			list_add(list, &item);
			it = nend;
			if(*it == ',') ++it;
		}
	}
	*ptr = it;
}

void list_parse(struct line_s* line, struct list_s* list)
{
	char const* pos;

	assert(line);
	assert(list);

	pos = line->m_elements;
	list_parse_r(&pos, list);
	if(pos != line->m_elements + line->m_count) crash();
}

int list_compare(struct list_s* a, struct list_s* b)
{
	int i;

	assert(a);
	assert(b);

	i = 0;
	for(;; ++i)
	{
		if(i != a->m_count && i != b->m_count)
		{
			if(a->m_elements[i].m_type == 0 && b->m_elements[i].m_type == 0)
			{
				if(a->m_elements[i].m_data.m_single < b->m_elements[i].m_data.m_single)
				{
					return 1;
				}
				else if(a->m_elements[i].m_data.m_single > b->m_elements[i].m_data.m_single)
				{
					return 0;
				}
			}
			else if(a->m_elements[i].m_type == 1 && b->m_elements[i].m_type == 1)
			{
				int cmp;
				cmp = list_compare(&a->m_elements[i].m_data.m_multi, &b->m_elements[i].m_data.m_multi);
				if(cmp != 2)
				{
					return cmp;
				}
			}
			else
			{
				struct list_s tmp;
				struct list_element_s item;
				int cmp;
				list_construct(&tmp);
				if(a->m_elements[i].m_type == 0)
				{
					item.m_type = 0;
					item.m_data.m_single = a->m_elements[i].m_data.m_single;
					list_add(&tmp, &item);
					cmp = list_compare(&tmp, &b->m_elements[i].m_data.m_multi);
				}
				else
				{
					item.m_type = 0;
					item.m_data.m_single = b->m_elements[i].m_data.m_single;
					list_add(&tmp, &item);
					cmp = list_compare(&a->m_elements[i].m_data.m_multi, &tmp);
				}
				list_destruct(&tmp);
				if(cmp != 2)
				{
					return cmp;
				}
			}
		}
		else if(a->m_count == i && i != b->m_count)
		{
			return 1;
		}
		else if(a->m_count != i && i == b->m_count)
		{
			return 0;
		}
		else if(a->m_count == i && i == b->m_count)
		{
			return 2;
		}
	}
	crash();
}

void mk_aoc2022_day13a(char const* const input, int* out)
{
	struct line_s line;
	FILE* file;
	int i;
	int sum;
	int err;
	struct list_s lista;
	struct list_s listb;
	int closed;

	assert(input);
	assert(*input);
	assert(out);

	mk_aoc2022_day05_line_construct(&line);
	file = fopen(input, "rb");
	if(!file) crash();
	i = 0;
	sum = 0;
	for(;;)
	{
		++i;
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count == 0) break;
		list_parse(&line, &lista);
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count == 0) crash();
		list_parse(&line, &listb);
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count != 0) crash();
		if(list_compare(&lista, &listb) == 1)
		{
			sum += i;
		}
		list_destruct(&lista);
		list_destruct(&listb);
	}
	mk_aoc2022_day05_line_destruct(&line);
	closed = fclose(file);
	if(closed != 0) crash();
	*out = sum;
}

struct lists_s
{
	int m_count;
	int m_capacity;
	struct list_s* m_elements;
};

void lists_construct(struct lists_s* lists)
{
	assert(lists);

	lists->m_count = 0;
	lists->m_capacity = 0;
	lists->m_elements = NULL;
}

void lists_destruct(struct lists_s* lists)
{
	int i;

	assert(lists);

	for(i = 0; i != lists->m_count; ++i)
	{
		list_destruct(&lists->m_elements[i]);
	}
	free(lists->m_elements);
}

void lists_add(struct lists_s* lists, struct list_s* list)
{
	struct list_s* nelems;

	assert(lists);
	assert(list);

	if(lists->m_count == lists->m_capacity)
	{
		lists->m_capacity *= 2;
		if(lists->m_capacity == 0) lists->m_capacity = 8;
		nelems = malloc(lists->m_capacity * sizeof(*nelems));
		if(!nelems) crash();
		memcpy(nelems, lists->m_elements, lists->m_count * sizeof(*nelems));
		free(lists->m_elements);
		lists->m_elements = nelems;
	}
	lists->m_elements[lists->m_count] = *list;
	++lists->m_count;
}

void lists_sort(struct lists_s* lists)
{
	int n;
	int changed;
	int i;
	int cmp;
	struct list_s tmp;

	assert(lists);

	n = lists->m_count;
	do
	{
		changed = 0;
		for(i = 0; i != n - 1; ++i)
		{
			cmp = list_compare(&lists->m_elements[i], &lists->m_elements[i + 1]);
			if(cmp == 0)
			{
				tmp = lists->m_elements[i];
				lists->m_elements[i] = lists->m_elements[i + 1];
				lists->m_elements[i + 1] = tmp;
				changed = 1;
			}
		}
	}while(changed != 0);
}

int lists_find(struct lists_s* lists, struct list_s* list)
{
	int n;
	int i;

	assert(lists);
	assert(list);

	n = lists->m_count;
	for(i = 0; i != n; ++i)
	{
		if
		(
			lists->m_elements[i].m_count == list->m_count &&
			lists->m_elements[i].m_capacity == list->m_capacity &&
			lists->m_elements[i].m_elements == list->m_elements
		)
		{
			return i + 1;
		}
	}
	crash();
	return -1;
}

void mk_aoc2022_day13b(char const* const input, int* out)
{
	struct lists_s lists;
	struct line_s line;
	FILE* file;
	int err;
	struct list_s list;
	int closed;
	struct list_s list2;
	struct list_element_s item;
	struct list_s list6;

	assert(input);
	assert(*input);
	assert(out);

	lists_construct(&lists);
	mk_aoc2022_day05_line_construct(&line);
	file = fopen(input, "rb");
	if(!file) crash();
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count == 0) break;
		list_parse(&line, &list);
		lists_add(&lists, &list);
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count == 0) crash();
		list_parse(&line, &list);
		lists_add(&lists, &list);
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count != 0) crash();
	}
	mk_aoc2022_day05_line_destruct(&line);
	closed = fclose(file);
	if(closed != 0) crash();
	list_construct(&list2);
	item.m_type = 0;
	item.m_data.m_single = 2;
	list_add(&list2, &item);
	lists_add(&lists, &list2);
	list_construct(&list6);
	item.m_type = 0;
	item.m_data.m_single = 6;
	list_add(&list6, &item);
	lists_add(&lists, &list6);
	lists_sort(&lists);
	*out = lists_find(&lists, &list2) * lists_find(&lists, &list6);
	lists_destruct(&lists);
}


void day14_parse(char const* const file_name, struct ints_s* values)
{
	struct line_s line;
	FILE* file;
	int err;
	char const* pos;
	char const* nend;
	int value;
	int closed;

	assert(file_name);
	assert(*file_name);
	assert(values);

	ints_construct(values);
	mk_aoc2022_day05_line_construct(&line);
	file = fopen(file_name, "rb");
	if(!file) crash();
	for(;;)
	{
		err = mk_aoc2022_day05_line_read(file, &line);
		if(err != 0) crash();
		if(line.m_count == 0) break;
		pos = line.m_elements;
		{
			parse_int(pos, line.m_elements + line.m_count, &err, &nend, &value);
			if(err != 0) crash();
			ints_append(values, value);
			if(*nend != ',') crash();
			pos = nend + 1;
			parse_int(pos, line.m_elements + line.m_count, &err, &nend, &value);
			if(err != 0) crash();
			ints_append(values, value);
			pos = nend;
		}
		for(;;)
		{
			if(pos == line.m_elements + line.m_count)
			{
				break;
			}
			if(!(pos <= line.m_elements + line.m_count - 4)) crash();
			if(memcmp(pos, " -> ", 4) != 0) crash();
			pos += 4;
			{
				parse_int(pos, line.m_elements + line.m_count, &err, &nend, &value);
				if(err != 0) crash();
				ints_append(values, value);
				if(*nend != ',') crash();
				pos = nend + 1;
				parse_int(pos, line.m_elements + line.m_count, &err, &nend, &value);
				if(err != 0) crash();
				ints_append(values, value);
				pos = nend;
				if
				(!(
					values->m_elements[values->m_count - 1] == values->m_elements[values->m_count - 3] ||
					values->m_elements[values->m_count - 2] == values->m_elements[values->m_count - 4]
				)) crash();
			}
		}
		ints_append(values, -1);
		ints_append(values, -1);
	}
	mk_aoc2022_day05_line_destruct(&line);
	closed = fclose(file);
	if(closed != 0) crash();
}

struct day14_sizes_s
{
	int m_minx;
	int m_maxx;
	int m_miny;
	int m_maxy;
};

void day14_build_grid(struct ints_s* values, struct grid_s* grid, struct day14_sizes_s* sizes, int algo)
{
	int i;
	int x;
	int y;
	int xx;
	int yy;
	int a;
	int b;
	int j;

	assert(values);
	assert(grid);
	assert(sizes);

	sizes->m_minx = INT_MAX;
	sizes->m_maxx = INT_MIN;
	sizes->m_miny = INT_MAX;
	sizes->m_maxy = INT_MIN;
	for(i = 0; i != values->m_count / 2; ++i)
	{
		x = values->m_elements[i * 2 + 0];
		y = values->m_elements[i * 2 + 1];
		if(x == -1 && y == -1) continue;
		if(x < sizes->m_minx) sizes->m_minx = x;
		if(x > sizes->m_maxx) sizes->m_maxx = x;
		if(y < sizes->m_miny) sizes->m_miny = y;
		if(y > sizes->m_maxy) sizes->m_maxy = y;
	}
	if(algo == 1)
	{
		ints_append(values, sizes->m_minx - (sizes->m_maxx - sizes->m_minx + 1) * 2 - (sizes->m_maxy - sizes->m_miny + 1) * 2); ints_append(values, sizes->m_maxy + 2);
		ints_append(values, sizes->m_maxx + (sizes->m_maxx - sizes->m_minx + 1) * 2 + (sizes->m_maxy - sizes->m_miny + 1) * 2); ints_append(values, sizes->m_maxy + 2);
		sizes->m_minx -= ((sizes->m_maxx - sizes->m_minx + 1) * 2 + (sizes->m_maxy - sizes->m_miny + 1) * 2);
		sizes->m_maxx += ((sizes->m_maxx - sizes->m_minx + 1) * 2 + (sizes->m_maxy - sizes->m_miny + 1) * 2);
		sizes->m_maxy += 2;
	}
	sizes->m_miny = 0;
	grid_construct(grid, sizes->m_maxx - sizes->m_minx + 1, sizes->m_maxy - sizes->m_miny + 1);
	grid_fill(grid, 0);
	xx = values->m_elements[0 * 2 + 0];
	yy = values->m_elements[0 * 2 + 1];
	for(j = 1; j != values->m_count / 2; ++j)
	{
		x = values->m_elements[j * 2 + 0];
		y = values->m_elements[j * 2 + 1];
		if(!((x == -1 && y == -1) || (xx == -1 && yy == -1)))
		{
			if(x == xx)
			{
				a = yy < y ? yy : y;
				b = yy < y ? y : yy;
				for(i = 0; i != b - a + 1; ++i)
				{
					grid_set(grid, x - sizes->m_minx, a + i - sizes->m_miny, 1);
				}
			}
			else if(y == yy)
			{
				a = xx < x ? xx : x;
				b = xx < x ? x : xx;
				for(i = 0; i != b - a + 1; ++i)
				{
					grid_set(grid, a + i - sizes->m_minx, y - sizes->m_miny, 1);
				}
			}
		}
		xx = x;
		yy = y;
	}
}

void visualize_sand(struct grid_s* grid, int idx)
{
	static char const s_p3_header[] = "P3\x0a";
	static char const s_p3_colors[] = "255\x0a";

	char name[64];
	FILE* file;
	int y;
	int x;
	int r;
	int item;
	int g;
	int b;
	int closed;

	sprintf(name, "%06d.ppm", idx);
	file = fopen(name, "wb");
	if(!file) crash();
	fwrite(s_p3_header, 1, sizeof(s_p3_header) / sizeof(*s_p3_header) - 1, file);
	fprintf(file, "%d %d\x0a", grid->m_width, grid->m_height);
	fwrite(s_p3_colors, 1, sizeof(s_p3_colors) / sizeof(*s_p3_colors) - 1, file);
	for(y = 0; y != grid->m_height; ++y)
	{
		for(x = 0; x != grid->m_width; ++x)
		{
			item = grid_get(grid, x, y);
			if(item == 0){ r = 0; g = 0; b = 0; }
			else if(item == 1){ r = 0; g = 255; b = 0; }
			else if(item == 2){ r = 255; g = 255; b = 0; }
			fprintf(file, "%d %d %d\x0a", r, g, b);
		}
	}
	closed = fclose(file);
	if(closed != 0) crash();
}

void day14_simulate_sand(struct grid_s* grid, struct day14_sizes_s* sizes, int* out)
{
	int final_pieces;
	int x;
	int y;

	assert(grid);
	assert(sizes);
	assert(out);

	final_pieces = 0;
	for(;;)
	{
		x = 500;
		y = 0;
		x = x - sizes->m_minx;
		y = y - sizes->m_miny;
		if(grid_get(grid, x, y) != 0)
		{
			visualize_sand(grid, final_pieces);
			*out = final_pieces;
			return;
		}
		for(;;)
		{
			if(y < -1)
			{
				++y;
			}
			else if(x < 0 || x == grid->m_width || y + 1 == grid->m_height)
			{
				visualize_sand(grid, final_pieces);
				*out = final_pieces;
				return;
			}
			else if(grid_get(grid, x, y + 1) == 0)
			{
				++y;
			}
			else if(x - 1 < 0)
			{
				visualize_sand(grid, final_pieces);
				*out = final_pieces;
				return;
			}
			else if(grid_get(grid, x - 1, y + 1) == 0)
			{
				--x;
				++y;
			}
			else if(x + 1 == grid->m_width)
			{
				visualize_sand(grid, final_pieces);
				*out = final_pieces;
				return;
			}
			else if(grid_get(grid, x + 1, y + 1) == 0)
			{
				++x;
				++y;
			}
			else
			{
				grid_set(grid, x, y, 2);
				/*visualize_sand(grid, final_pieces);*/
				++final_pieces;
				break;
			}
		}
	}
}

void mk_aoc2022_day14a(char const* const input, int* out)
{
	struct ints_s values;
	struct grid_s grid;
	struct day14_sizes_s sizes;

	assert(input);
	assert(*input);
	assert(out);

	day14_parse(input, &values);
	day14_build_grid(&values, &grid, &sizes, 0);
	ints_destruct(&values);
	day14_simulate_sand(&grid, &sizes, out);
	grid_destruct(&grid);
}

void mk_aoc2022_day14b(char const* const input, int* out)
{
	struct ints_s values;
	struct grid_s grid;
	struct day14_sizes_s sizes;

	assert(input);
	assert(*input);
	assert(out);

	day14_parse(input, &values);
	day14_build_grid(&values, &grid, &sizes, 1);
	ints_destruct(&values);
	day14_simulate_sand(&grid, &sizes, out);
	grid_destruct(&grid);
}


#include <stdio.h>

int main(void)
{
	int err;
	int ret;
	long long int llret;

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

	printf("%s\n", "Day 5");
	err = mk_aoc2022_day05a("input05a.txt");
	if(err != 0) return err;
	err = mk_aoc2022_day05a("input05b.txt");
	if(err != 0) return err;
	err = mk_aoc2022_day05b("input05a.txt");
	if(err != 0) return err;
	err = mk_aoc2022_day05b("input05b.txt");
	if(err != 0) return err;

	printf("%s\n", "Day 6");
	err = mk_aoc2022_day06a("input06a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06a("input06b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06a("input06c.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06a("input06d.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06a("input06e.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06a("input06f.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06b("input06a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06b("input06b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06b("input06c.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06b("input06d.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06b("input06e.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day06b("input06f.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	printf("%s\n", "Day 7");
	err = mk_aoc2022_day07a("input07a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day07a("input07b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day07b("input07a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day07b("input07b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	printf("%s\n", "Day 8");
	err = mk_aoc2022_day08a("input08a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day08a("input08b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day08b("input08a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day08b("input08b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	printf("%s\n", "Day 9");
	err = mk_aoc2022_day09a("input09a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day09a("input09b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day09b("input09a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day09b("input09c.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day09b("input09b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);

	printf("%s\n", "Day 10");
	err = mk_aoc2022_day10a("input10a.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day10a("input10b.txt", &ret);
	if(err != 0) return err;
	printf("%d\n", ret);
	err = mk_aoc2022_day10b("input10a.txt", &ret);
	if(err != 0) return err;
	err = mk_aoc2022_day10b("input10b.txt", &ret);
	if(err != 0) return err;

	printf("%s\n", "Day 11");
	err = mk_aoc2022_day11a("input11a.txt", &llret);
	if(err != 0) return err;
	printf("%lld\n", llret);
	err = mk_aoc2022_day11a("input11b.txt", &llret);
	if(err != 0) return err;
	printf("%lld\n", llret);
	err = mk_aoc2022_day11b("input11a.txt", &llret);
	if(err != 0) return err;
	printf("%lld\n", llret);
	err = mk_aoc2022_day11b("input11b.txt", &llret);
	if(err != 0) return err;
	printf("%lld\n", llret);

	printf("%s\n", "Day 12");
	mk_aoc2022_day12a("input12a.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day12a("input12b.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day12b("input12a.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day12b("input12b.txt", &ret);
	printf("%d\n", ret);

	printf("%s\n", "Day 13");
	mk_aoc2022_day13a("input13a.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day13a("input13b.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day13b("input13a.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day13b("input13b.txt", &ret);
	printf("%d\n", ret);

	printf("%s\n", "Day 14");
	mk_aoc2022_day14a("input14a.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day14a("input14b.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day14b("input14a.txt", &ret);
	printf("%d\n", ret);
	mk_aoc2022_day14b("input14b.txt", &ret);
	printf("%d\n", ret);

	return 0;
}
