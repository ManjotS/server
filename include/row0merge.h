/******************************************************
Index build routines using a merge sort

(c) 2005 Innobase Oy

Created 13/06/2005 Jan Lindstrom
*******************************************************/

#ifndef row0merge_h
#define row0merge_h

#include "univ.i"
#include "data0data.h"
#include "dict0types.h"
#include "trx0types.h"
#include "que0types.h"
#include "mtr0mtr.h"
#include "rem0types.h"
#include "rem0rec.h"
#include "read0types.h"
#include "btr0types.h"
#include "row0mysql.h"

/* This structure holds index field definitions */

struct merge_index_field_struct {
	ulint		prefix_len;	/* Prefix len */
	const char*	field_name;	/* Field name */
};

typedef struct merge_index_field_struct merge_index_field_t;

/* This structure holds index definitions */

struct merge_index_def_struct {
	const char*		name;		/* Index name */
	ulint			ind_type;	/* 0, DICT_UNIQUE,
						or DICT_CLUSTERED */
	ulint			n_fields;	/* Number of fields in index */
	merge_index_field_t*	fields;		/* Field definitions */
};

typedef struct merge_index_def_struct merge_index_def_t;

/*************************************************************************
Drop an index from the InnoDB system tables. */

void
row_merge_drop_index(
/*=================*/
	dict_index_t*	index,	/* in: index to be removed */
	dict_table_t*	table,	/* in: table */
	trx_t*		trx);	/* in: transaction handle */
/*************************************************************************
Drop those indexes which were created before an error occurred
when building an index. */

void
row_merge_drop_indexes(
/*===================*/
	trx_t*		trx,		/* in: transaction */
	dict_table_t*	table,		/* in: table containing the indexes */
	dict_index_t**	index,		/* in: indexes to drop */
	ulint		num_created);	/* in: number of elements in index[] */

/*************************************************************************
Create a temporary table for creating a primary key, using the definition
of an existing table. */

dict_table_t*
row_merge_create_temporary_table(
/*=============================*/
						/* out: table,
						or NULL on error */
	const char*		table_name,	/* in: new table name */
	const merge_index_def_t*index_def,	/* in: the index definition
						of the primary key */
	const dict_table_t*	table,		/* in: old table definition */
	trx_t*			trx);		/* in/out: transaction
						(sets error_state) */
/*************************************************************************
Rename the indexes in the dictionary. */

ulint
row_merge_rename_index(
/*===================*/
					/* out: DB_SUCCESS if all OK */
	trx_t*		trx,		/* in: Transaction */
	dict_table_t*	table,		/* in: Table for index */
	dict_index_t*	index);		/* in: Index to rename */
/*************************************************************************
Create the index and load in to the dictionary. */

dict_index_t*
row_merge_create_index(
/*===================*/
					/* out: index, or NULL on error */
	trx_t*		trx,		/* in/out: trx (sets error_state) */
	dict_table_t*	table,		/* in: the index is on this table */
	const merge_index_def_t*	/* in: the index definition */
			index_def);
/*************************************************************************
Check if a transaction can use an index. */

ibool
row_merge_is_index_usable(
/*======================*/
					/* out: TRUE if index can be used by
					the transaction else FALSE*/
	const trx_t*		trx,	/* in: transaction */
	const dict_index_t*	index);	/* in: index to check */
/*************************************************************************
If there are views that refer to the old table name then we "attach" to
the new instance of the table else we drop it immediately. */

ulint
row_merge_drop_table(
/*=================*/
					/* out: DB_SUCCESS or error code */
	trx_t*		trx,		/* in: transaction */
	dict_table_t*	table);		/* in: table instance to drop */

/*************************************************************************
Build indexes on a table by reading a clustered index,
creating a temporary file containing index entries, merge sorting
these index entries and inserting sorted index entries to indexes. */

ulint
row_merge_build_indexes(
/*====================*/
					/* out: DB_SUCCESS or error code */
	trx_t*		trx,		/* in: transaction */
	dict_table_t*	old_table,	/* in: table where rows are
					read from */
	dict_table_t*	new_table,	/* in: table where indexes are
					created; identical to old_table
					unless creating a PRIMARY KEY */
	dict_index_t**	indexes,	/* in: indexes to be created */
	ulint		n_indexes);	/* in: size of indexes[] */
#endif /* row0merge.h */
