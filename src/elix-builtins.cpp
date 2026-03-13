#include "elix.hpp"

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -


static Object fn_symbol(const Vec<Object>& args);
static Object fn_string(const Vec<Object>& args);
static Object fn_integer(const Vec<Object>& args);
static Object fn_float(const Vec<Object>& args);
static Object fn_array(const Vec<Object>& args);
static Object fn_hashmap(const Vec<Object>& args);
static Object fn_hashset(const Vec<Object>& args);
static Object fn_list(const Vec<Object>& args);
static Object fn_pair(const Vec<Object>& args);
static Object fn_typeof(const Vec<Object>& args);
static Object fn_contains(const Vec<Object>& args);
static Object fn_isinstance(const Vec<Object>& args);
static Object fn_is(const Vec<Object>& args);
static Object fn_symbols(const Vec<Object>& args);

static Object fn_add(const Vec<Object>& args);
static Object fn_sub(const Vec<Object>& args);
static Object fn_mul(const Vec<Object>& args);
static Object fn_div(const Vec<Object>& args);
static Object fn_mod(const Vec<Object>& args);
static Object fn_band(const Vec<Object>& args);
static Object fn_bor(const Vec<Object>& args);
static Object fn_bxor(const Vec<Object>& args);
static Object fn_not(const Vec<Object>& args);
static Object fn_and(const Vec<Object>& args);
static Object fn_or(const Vec<Object>& args);
static Object fn_lt(const Vec<Object>& args);
static Object fn_le(const Vec<Object>& args);
static Object fn_gt(const Vec<Object>& args);
static Object fn_ge(const Vec<Object>& args);
static Object fn_eq(const Vec<Object>& args);
static Object fn_ne(const Vec<Object>& args);

static Object fn_pair_first(const Vec<Object>& args);
static Object fn_pair_second(const Vec<Object>& args);

static Object fn_symbol_eq(const Vec<Object>& args);
static Object fn_symbol_ne(const Vec<Object>& args);

static Object fn_clone(const Vec<Object>& args);
static Object fn_map(const Vec<Object>& args);
static Object fn_filter(const Vec<Object>& args);
static Object fn_zip(const Vec<Object>& args);
static Object fn_enumerate(const Vec<Object>& args);
static Object fn_len(const Vec<Object>& args);
static Object fn_concat(const Vec<Object>& args);
static Object fn_reverse(const Vec<Object>& args);
static Object fn_find(const Vec<Object>& args);

static Object fn_println(const Vec<Object>& args);
static Object fn_eprintln(const Vec<Object>& args);
static Object fn_print(const Vec<Object>& args);
static Object fn_eprint(const Vec<Object>& args);
static Object fn_panic(const Vec<Object>& args);
static Object fn_input(const Vec<Object>& args);
static Object fn_format(const Vec<Object>& args);

static Object fn_min(const Vec<Object>& args);
static Object fn_max(const Vec<Object>& args);
static Object fn_range(const Vec<Object>& args);
static Object fn_linspace(const Vec<Object>& args);

static Object fn_parse(const Vec<Object>& args);
static Object fn_eval(const Vec<Object>& args);
static Object fn_import(const Vec<Object>& args);

static Object fn_str_find(const Vec<Object>& args);
static Object fn_str_reverse(const Vec<Object>& args);
static Object fn_str_concat(const Vec<Object>& args);
static Object fn_str_len(const Vec<Object>& args);
static Object fn_str_eq(const Vec<Object>& args);
static Object fn_str_ne(const Vec<Object>& args);
static Object fn_str_split(const Vec<Object>& args);
static Object fn_str_slice(const Vec<Object>& args);
static Object fn_str_startswith(const Vec<Object>& args);
static Object fn_str_endswith(const Vec<Object>& args);
static Object fn_str_ltrim(const Vec<Object>& args);
static Object fn_str_rtrim(const Vec<Object>& args);
static Object fn_str_trim(const Vec<Object>& args);
static Object fn_str_lower(const Vec<Object>& args);
static Object fn_str_upper(const Vec<Object>& args);
static Object fn_str_capitalize(const Vec<Object>& args);
static Object fn_str_title(const Vec<Object>& args);
static Object fn_str_find_all(const Vec<Object>& args);
static Object fn_str_replace(const Vec<Object>& args);
static Object fn_str_replace_all(const Vec<Object>& args);
static Object fn_str_chr(const Vec<Object>& args);
static Object fn_str_ord(const Vec<Object>& args);
static Object fn_str_substr(const Vec<Object>& args);

static Object fn_array_find(const Vec<Object>& args);
static Object fn_array_reverse(const Vec<Object>& args);
static Object fn_array_concat(const Vec<Object>& args);
static Object fn_array_len(const Vec<Object>& args);
static Object fn_array_slice(const Vec<Object>& args);
static Object fn_array_insert(const Vec<Object>& args);
static Object fn_array_get(const Vec<Object>& args);
static Object fn_array_set(const Vec<Object>& args);
static Object fn_array_splice(const Vec<Object>& args);
static Object fn_array_any(const Vec<Object>& args);
static Object fn_array_all(const Vec<Object>& args);
static Object fn_array_sort(const Vec<Object>& args);
static Object fn_array_reduce(const Vec<Object>& args);

static Object fn_list_find(const Vec<Object>& args);
static Object fn_list_reverse(const Vec<Object>& args);
static Object fn_list_concat(const Vec<Object>& args);
static Object fn_list_len(const Vec<Object>& args);
static Object fn_list_head(const Vec<Object>& args);
static Object fn_list_tail(const Vec<Object>& args);
static Object fn_list_first(const Vec<Object>& args);
static Object fn_list_last(const Vec<Object>& args);
static Object fn_list_push(const Vec<Object>& args);
static Object fn_list_pop(const Vec<Object>& args);
static Object fn_list_insert(const Vec<Object>& args);
static Object fn_list_remove(const Vec<Object>& args);

static Object fn_dict_find(const Vec<Object>& args);
static Object fn_dict_concat(const Vec<Object>& args);
static Object fn_dict_len(const Vec<Object>& args);
static Object fn_dict_get(const Vec<Object>& args);
static Object fn_dict_set(const Vec<Object>& args);
static Object fn_dict_update(const Vec<Object>& args);
static Object fn_dict_keys(const Vec<Object>& args);
static Object fn_dict_values(const Vec<Object>& args);
static Object fn_dict_popitem(const Vec<Object>& args);
static Object fn_dict_clear(const Vec<Object>& args);
static Object fn_dict_items(const Vec<Object>& args);

static Object fn_hashset_find(const Vec<Object>& args);
static Object fn_hashset_concat(const Vec<Object>& args);
static Object fn_hashset_len(const Vec<Object>& args);
static Object fn_hashset_add(const Vec<Object>& args);
static Object fn_hashset_union(const Vec<Object>& args);
static Object fn_hashset_intersection(const Vec<Object>& args);
static Object fn_hashset_symmetric_difference(const Vec<Object>& args);
static Object fn_hashset_clear(const Vec<Object>& args);
static Object fn_hashset_discard(const Vec<Object>& args);
static Object fn_hashset_pop(const Vec<Object>& args);
static Object fn_hashset_isdisjoint(const Vec<Object>& args);
static Object fn_hashset_issubset(const Vec<Object>& args);
static Object fn_hashset_issuperset(const Vec<Object>& args);


// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-