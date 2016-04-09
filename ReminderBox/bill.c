#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "memory.h"
#include "bill.h"


#define BILL_LENGTH 4//A bill is four words: company, person, month, day
static uint16_t bill_as_words[BILL_LENGTH];


void bill_write_to_memory(bill_t *bill)
{
	bill_as_words[0] = (uint16_t)bill->company;
	bill_as_words[1] = (uint16_t)bill->person;
	bill_as_words[2] = (uint16_t)bill->date.month;
	bill_as_words[3] = (uint16_t)bill->date.day;

	memory_write_words(bill_as_words, BILL_LENGTH);
}

void bill_read_next_from_memory(bill_t *bill)
{
	memory_read_words_no_addr(bill_as_words, BILL_LENGTH, true);
	bill->company = (company_e)bill_as_words[0];
	bill->person = (person_e)bill_as_words[1];
	bill->date.month = (month_e)bill_as_words[2];
	bill->date.day = (uint8_t)bill_as_words[3];
}

void bill_read_last_from_memory(bill_t *bill)
{
	memory_read_words_no_addr(bill_as_words, BILL_LENGTH, false);
	bill->company = (company_e)bill_as_words[3];
	bill->person = (person_e)bill_as_words[2];
	bill->date.month = (month_e)bill_as_words[1];
	bill->date.day = (uint8_t)bill_as_words[0];
}


