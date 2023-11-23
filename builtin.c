/* ******************************* builtin.c ******************************** */
/*                                                                            */
/*  Vytvořil a zpracoval: Lukáš Witpeerd, listopad 2023                       */
/*
 **/

#include "builtin.h"

void builtin_add_to_symtable(Symtable *symtable) {
  if (symtable == NULL) {
    return;
  }

  SymtableItem *item = NULL;
  SymtableIdentifierType identifier_type;

  /*                              readString() -> String?                      */
  item = symtable_add_symbol(symtable, "readString", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = STRING_TYPE;
  identifier_type.nullable = true;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------ */

  /*                             readInt() -> Int?                             */
  item = symtable_add_symbol(symtable, "readInt", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = true;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */

  /*                            readDouble() -> Double?                        */
  item = symtable_add_symbol(symtable, "readDouble", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = DOUBLE_TYPE;
  identifier_type.nullable = true;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */

  /*                         write(term1, term2, ..., termn)                   */
  item = symtable_add_symbol(symtable, "write", SYMTABLE_FUNCTION, true, false);

  // ????? args
  /* ------------------------------------------------------------------------- */

  /*                       Int2Double(_ term: Int) -> Double                   */
  item = symtable_add_symbol(symtable, "Int2Double", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "_", "term", identifier_type);

  identifier_type.data_type = DOUBLE_TYPE;
  identifier_type.nullable = false;
  symtable_add_return(item, identifier_type);
  /* -------------------------------------------------------------------------- */

  /*                     Double2Int(_ term: Double) -> Int                      */
  item = symtable_add_symbol(symtable, "Double2Int", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = DOUBLE_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "_", "term", identifier_type);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */

  /*                       Length(_ s: String) -> Int                          */
  item = symtable_add_symbol(symtable, "length", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = STRING_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "_", "s", identifier_type);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */

  /* substring(of s: String, startingAt i: Int, endingBefore j: Int) -> String */
  item = symtable_add_symbol(symtable, "substring", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = STRING_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "of", "s", identifier_type);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "startingAt", "i", identifier_type);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "endingBefore", "j", identifier_type);

  identifier_type.data_type = STRING_TYPE;
  identifier_type.nullable = false;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */

  /*                       ord(_ c: String) -> Int                             */
  item = symtable_add_symbol(symtable, "ord", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = STRING_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "_", "c", identifier_type);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */

  /*                         chr(_ i: Int) -> String                           */
  item = symtable_add_symbol(symtable, "chr", SYMTABLE_FUNCTION, true, false);

  identifier_type.data_type = INT_TYPE;
  identifier_type.nullable = false;
  symtable_add_param(item, "_", "i", identifier_type);

  identifier_type.data_type = STRING_TYPE;
  identifier_type.nullable = false;
  symtable_add_return(item, identifier_type);
  /* ------------------------------------------------------------------------- */
}