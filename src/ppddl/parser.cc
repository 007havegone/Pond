/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 20 "parser.yy"

#include <config.h>
#include "problems.h"
#include "domains.h"
#include "actions.h"
#include "effects.h"
#include "formulas.h"
#include "expressions.h"
#include "functions.h"
#include "predicates.h"
#include "terms.h"
#include "types.h"
#include "rational.h"
#include "exceptions.h"
#include "observations.h"
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

  //dan
#include "globals.h"
  //#include <FlexLexer.h>


/* Workaround for bug in Bison 1.35 that disables stack growth. */
#define YYLTYPE_IS_TRIVIAL 1


/*
 * Context of free variables.
 */
struct Context {
  void push_frame() {
    frames_.push_back(VariableMap());
  }

  void pop_frame() {
    frames_.pop_back();
  }

  void insert(const std::string& name, Variable v) {
    frames_.back()[name] = v;
  }

  std::pair<Variable, bool> shallow_find(const std::string& name) const {
    VariableMap::const_iterator vi = frames_.back().find(name);
    if (vi != frames_.back().end()) {
      return std::make_pair((*vi).second, true);
    } else {
      return std::make_pair(0, false);
    }
  }

  std::pair<Variable, bool> find(const std::string& name) const {
    for (std::vector<VariableMap>::const_reverse_iterator fi =
	   frames_.rbegin(); fi != frames_.rend(); fi++) {
      VariableMap::const_iterator vi = (*fi).find(name);
      if (vi != (*fi).end()) {
	return std::make_pair((*vi).second, true);
      }
    }
    return std::make_pair(0, false);
  }

private:
  struct VariableMap : public std::map<std::string, Variable> {
  };

  std::vector<VariableMap> frames_;
};

//DAN
 extern int yyrestart(FILE*);
 // extern int yyrestart();
 // extern int yy_scan_string(char*);


/* The lexer. */
extern int yylex();
/* Current line number. */
extern size_t line_number;
/* Name of current file. */
extern std::string current_file;
/* Level of warnings. */
extern int warning_level;

/* Whether the last parsing attempt succeeded. */
static bool success = true;
/* Current domain. */
static Domain* domain;
/* Domains. */
static std::map<std::string, Domain*> domains;
/* Problem being parsed, or NULL if no problem is being parsed. */
static Problem* problem;
/* Current requirements. */
static Requirements* requirements;
/* The goal probability function, if probabilistic effects are required. */
static Function goal_prob_function; 
/* The reward function, if rewards are required. */
static Function reward_function;
/* Predicate being parsed. */
static Predicate predicate;
/* Whether a predicate is being parsed. */
static bool parsing_predicate;
/* Whether predicate declaration is repeated. */
static bool repeated_predicate;
/* Function being parsed. */
static Function function;
/* Whether a function is being parsed. */
static bool parsing_function;
/* Whether function declaration is repeated. */
static bool repeated_function;
/* Action being parsed, or NULL if no action is being parsed. */
static ActionSchema* action;
/* Current variable context. */
static Context context;
/* Predicate for atomic state formula being parsed. */
static Predicate atom_predicate;
/* Whether the predicate of the currently parsed atom was undeclared. */
static bool undeclared_atom_predicate;
/* Whether parsing effect fluents. */
static bool effect_fluent;
/* Whether parsing metric fluent. */
static bool metric_fluent;
/* Function for function application being parsed. */
static Function appl_function;
/* Whether the function of the currently parsed application was undeclared. */
static bool undeclared_appl_function;
/* Paramerers for atomic state formula or function application being parsed. */
static TermList term_parameters;
/* Whether parsing an atom. */
static bool parsing_atom;
/* Whether parsing a function application. */
static bool parsing_application;
/* Whether parsing a obs token. */
static bool parsing_obs_token = false;
/* Quantified variables for effect or formula being parsed. */
static VariableList quantified;
/* Most recently parsed term for equality formula. */
static Term eq_term;
/* Most recently parsed expression for equality formula. */
static const Expression* eq_expr;
/* The first term for equality formula. */
static Term first_eq_term;
/* The first expression for equality formula. */
static const Expression* first_eq_expr;
/* Kind of name map being parsed. */
static enum { TYPE_KIND, CONSTANT_KIND, OBJECT_KIND, VOID_KIND } name_kind;

/* Outputs an error message. */
static void yyerror(const std::string& s); 
/* Outputs a warning message. */
static void yywarning(const std::string& s);
/* Creates an empty domain with the given name. */
static void make_domain(const std::string* name);
/* Creates an empty problem with the given name. */
static void make_problem(const std::string* name,
			 const std::string* domain_name);
/* Adds :typing to the requirements. */
static void require_typing();
/* Adds :fluents to the requirements. */
static void require_fluents();
/* Adds :disjunctive-preconditions to the requirements. */
static void require_disjunction();
/* Adds :conditional-effects to the requirements. */ 
static void require_conditional_effects();
/* Returns a simple type with the given name. */
static Type make_type(const std::string* name);
/* Returns the union of the given types. */
static Type make_type(const TypeSet& types);
/* Returns a simple term with the given name. */
static Term make_term(const std::string* name);
/* Creates a predicate with the given name. */
static void make_predicate(const std::string* name);
/* Creates an observation token with the given name. */
static void make_observable(const std::string* name);
/* Creates a function with the given name. */
static void make_function(const std::string* name);
/* Creates an action with the given name. */
 static ObservationEntry* make_observation(const StateFormula&, const StateFormula&, const Rational&);
 static ObservationEntry* make_observation(const StateFormula&, const Rational&, const Rational &);
 static ObservationEntry* make_observation(const StateFormula&, const ProbabilisticEffect&);
/* Creates an action with the given name. */
static void make_action(const std::string* name);
/* Adds the current action to the current domain. */
static void add_action();
/* Adds the current event to the current domain. */
static void add_event();
/* Prepares for the parsing of a universally quantified effect. */ 
static void prepare_forall_effect();
/* Creates a universally quantified effect. */
static const pEffect* make_forall_effect(const pEffect& effect);
/* Adds an outcome to the given probabilistic effect. */
static void add_effect_outcome(ProbabilisticEffect& peffect,
			        const Rational* p, 
			       const pEffect& effect);
//dan
static void add_feffect_outcome(ProbabilisticEffect& peffect,
			        const Expression* p, 
			       const pEffect& effect);
/* Creates an add effect. */
static const pEffect* make_add_effect(const Atom& atom);
/* Creates a delete effect. */
static const pEffect* make_delete_effect(const Atom& atom);
/* Creates an assignment effect. */
static const pEffect* make_assignment_effect(Assignment::AssignOp oper,
					    const Application& application,
					    const Expression& expr);
/* Adds types, constants, or objects to the current domain or problem. */
static void add_names(const std::vector<const std::string*>* names, Type type);
/* Adds variables to the current variable list. */
static void add_variables(const std::vector<const std::string*>* names,
			  Type type);
/* Prepares for the parsing of an atomic state formula. */ 
static void prepare_atom(const std::string* name);
/* Prepares for the parsing of a function application. */ 
static void prepare_application(const std::string* name);
/* Adds a term with the given name to the current atomic state formula. */
static void add_term(const std::string* name);
/* Creates the atomic formula just parsed. */
static const Atom* make_atom();
/* Creates the function application just parsed. */
static const Application* make_application();
/* Creates a subtraction. */
static const Expression* make_subtraction(const Expression& term,
					  const Expression* opt_term);
/* Creates an atom or fluent for the given name to be used in an
   equality formula. */
static void make_eq_name(const std::string* name);
/* Creates an equality formula. */
static const StateFormula* make_equality();
/* Creates a negated formula. */
static const StateFormula* make_negation(const StateFormula& negand);
/* Creates an implication. */
static const StateFormula* make_implication(const StateFormula& f1,
					    const StateFormula& f2);
/* Prepares for the parsing of an existentially quantified formula. */
static void prepare_exists();
/* Prepares for the parsing of a universally quantified formula. */
static void prepare_forall();
/* Creates an existentially quantified formula. */
static const StateFormula* make_exists(const StateFormula& body);
/* Creates a universally quantified formula. */
static const StateFormula* make_forall(const StateFormula& body);
/* Sets the goal reward for the current problem. */
static void set_goal_reward(const Expression& goal_reward);
/* Sets the default metric for the current problem. */
static void set_default_metric();
/* if we use logical formula for init, then need to get atoms holding for later grounding 
   purposes*/
 static void get_init_elts();
 static void set_discount(const Rational& discount);
/*DAN */



#line 328 "parser.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_HH_INCLUDED
# define YY_YY_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DEFINE = 258,
    DOMAIN_TOKEN = 259,
    PROBLEM = 260,
    REQUIREMENTS = 261,
    TYPES = 262,
    CONSTANTS = 263,
    PREDICATES = 264,
    FUNCTIONS = 265,
    OBSERVABLES = 266,
    STRIPS = 267,
    TYPING = 268,
    NEGATIVE_PRECONDITIONS = 269,
    DISJUNCTIVE_PRECONDITIONS = 270,
    EQUALITY = 271,
    EXISTENTIAL_PRECONDITIONS = 272,
    UNIVERSAL_PRECONDITIONS = 273,
    QUANTIFIED_PRECONDITIONS = 274,
    CONDITIONAL_EFFECTS = 275,
    FLUENTS = 276,
    ADL = 277,
    DURATIVE_ACTIONS = 278,
    DURATION_INEQUALITIES = 279,
    CONTINUOUS_EFFECTS = 280,
    PROBABILISTIC_EFFECTS = 281,
    REWARDS = 282,
    MDP = 283,
    ONEOF = 284,
    UNKNOWN = 285,
    NON_DETERMINISTIC_DYNAMICS = 286,
    PROBABILISTIC_DYNAMICS = 287,
    ACTION = 288,
    EVENT = 289,
    PARAMETERS = 290,
    PRECONDITION = 291,
    EFFECT = 292,
    OBSERVATION = 293,
    PDOMAIN = 294,
    OBJECTS = 295,
    INIT = 296,
    GOAL = 297,
    GOAL_REWARD = 298,
    METRIC = 299,
    GOAL_PROBABILITY = 300,
    WHEN = 301,
    NOT = 302,
    AND = 303,
    OR = 304,
    IMPLY = 305,
    EXISTS = 306,
    FORALL = 307,
    PROBABILISTIC = 308,
    ASSIGN = 309,
    SCALE_UP = 310,
    SCALE_DOWN = 311,
    INCREASE = 312,
    DECREASE = 313,
    MINIMIZE = 314,
    MAXIMIZE = 315,
    NUMBER_TOKEN = 316,
    OBJECT_TOKEN = 317,
    EITHER = 318,
    LE = 319,
    GE = 320,
    NAME = 321,
    VARIABLE = 322,
    NUMBER = 323,
    HORIZON = 324,
    DISC = 325,
    ILLEGAL_TOKEN = 326,
    PLANTIME = 327,
    PLAN = 328,
    FORPROBLEM = 329,
    IF = 330,
    THEN = 331,
    ELSE = 332,
    CASE = 333,
    GOTO = 334,
    DONE = 335,
    ANTI_COMMENT = 336
  };
#endif
/* Tokens.  */
#define DEFINE 258
#define DOMAIN_TOKEN 259
#define PROBLEM 260
#define REQUIREMENTS 261
#define TYPES 262
#define CONSTANTS 263
#define PREDICATES 264
#define FUNCTIONS 265
#define OBSERVABLES 266
#define STRIPS 267
#define TYPING 268
#define NEGATIVE_PRECONDITIONS 269
#define DISJUNCTIVE_PRECONDITIONS 270
#define EQUALITY 271
#define EXISTENTIAL_PRECONDITIONS 272
#define UNIVERSAL_PRECONDITIONS 273
#define QUANTIFIED_PRECONDITIONS 274
#define CONDITIONAL_EFFECTS 275
#define FLUENTS 276
#define ADL 277
#define DURATIVE_ACTIONS 278
#define DURATION_INEQUALITIES 279
#define CONTINUOUS_EFFECTS 280
#define PROBABILISTIC_EFFECTS 281
#define REWARDS 282
#define MDP 283
#define ONEOF 284
#define UNKNOWN 285
#define NON_DETERMINISTIC_DYNAMICS 286
#define PROBABILISTIC_DYNAMICS 287
#define ACTION 288
#define EVENT 289
#define PARAMETERS 290
#define PRECONDITION 291
#define EFFECT 292
#define OBSERVATION 293
#define PDOMAIN 294
#define OBJECTS 295
#define INIT 296
#define GOAL 297
#define GOAL_REWARD 298
#define METRIC 299
#define GOAL_PROBABILITY 300
#define WHEN 301
#define NOT 302
#define AND 303
#define OR 304
#define IMPLY 305
#define EXISTS 306
#define FORALL 307
#define PROBABILISTIC 308
#define ASSIGN 309
#define SCALE_UP 310
#define SCALE_DOWN 311
#define INCREASE 312
#define DECREASE 313
#define MINIMIZE 314
#define MAXIMIZE 315
#define NUMBER_TOKEN 316
#define OBJECT_TOKEN 317
#define EITHER 318
#define LE 319
#define GE 320
#define NAME 321
#define VARIABLE 322
#define NUMBER 323
#define HORIZON 324
#define DISC 325
#define ILLEGAL_TOKEN 326
#define PLANTIME 327
#define PLAN 328
#define FORPROBLEM 329
#define IF 330
#define THEN 331
#define ELSE 332
#define CASE 333
#define GOTO 334
#define DONE 335
#define ANTI_COMMENT 336

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 295 "parser.yy"

  Assignment::AssignOp setop;
  const pEffect* effect;
  ConjunctiveEffect* ceffect;
  ProbabilisticEffect* peffect; 
  Observation* observation_defs;
  ObservationEntry* observation;
  const StateFormula* formula;
  const Atom* atom;
  Conjunction* conj;
  Disjunction* disj;
  OneOfDisjunction* odisj;
  const Expression* expr;
  const Application* appl;
  Comparison::CmpPredicate comp;
  Type type;
  TypeSet* types;
  const std::string* str;
  std::vector<const std::string*>* strs;
  const Rational* num;
  //DAN
  plan* t_plan;
  Instruction * t_instr;
  Guards *t_guards;
  label_symbol* t_label_symbol;

  //NAD

#line 571 "parser.cc"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HH_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1789

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  185
/* YYNRULES -- Number of rules.  */
#define YYNRULES  409
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  696

#define YYUNDEFTOK  2
#define YYMAXUTOK   336


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      82,    83,    89,    88,     2,    84,     2,    90,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      86,    85,    87,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   356,   356,   356,   358,   358,   359,   359,   369,   377,
     387,   393,   395,   402,   409,   410,   411,   412,   413,   418,
     424,   427,   446,   447,   450,   457,   464,   469,   476,   483,
     489,   493,   500,   508,   509,   510,   517,   517,   521,   522,
     523,   524,   527,   528,   529,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   543,   544,   545,   546,   547,   548,
     549,   552,   553,   554,   555,   556,   559,   560,   561,   562,
     563,   564,   567,   568,   569,   572,   573,   574,   575,   576,
     579,   580,   581,   584,   584,   589,   590,   593,   596,   599,
     600,   603,   604,   605,   607,   609,   610,   612,   614,   616,
     617,   618,   620,   621,   622,   624,   626,   628,   636,   641,
     651,   651,   655,   655,   659,   662,   662,   669,   670,   673,
     673,   677,   678,   679,   682,   683,   686,   686,   689,   689,
     693,   694,   697,   697,   706,   706,   709,   709,   714,   715,
     718,   719,   722,   723,   724,   727,   728,   732,   735,   738,
     739,   745,   746,   747,   747,   749,   749,   751,   752,   753,
     754,   757,   758,   761,   766,   768,   774,   781,   783,   787,
     789,   793,   800,   803,   804,   805,   805,   809,   810,   811,
     812,   813,   819,   820,   823,   824,   825,   842,   841,   845,
     844,   851,   852,   856,   859,   860,   861,   862,   866,   867,
     868,   871,   872,   875,   876,   880,   881,   882,   885,   886,
     889,   890,   894,   895,   896,   899,   900,   903,   904,   908,
     908,   912,   913,   917,   919,   918,   924,   925,   928,   929,
     931,   933,   934,   935,   945,   950,   954,   956,   960,   965,
     966,   969,   970,   973,   974,   978,   981,   982,   983,   984,
     987,   988,   991,   992,   996,   997,   997,   999,   999,  1007,
    1009,  1008,  1011,  1011,  1013,  1014,  1015,  1015,  1016,  1016,
    1017,  1018,  1018,  1020,  1020,  1024,  1025,  1028,  1029,  1031,
    1032,  1035,  1035,  1037,  1040,  1040,  1042,  1045,  1046,  1047,
    1048,  1055,  1056,  1057,  1058,  1059,  1060,  1063,  1065,  1065,
    1067,  1067,  1069,  1069,  1071,  1071,  1073,  1073,  1075,  1076,
    1079,  1080,  1083,  1083,  1085,  1088,  1089,  1091,  1093,  1095,
    1097,  1098,  1102,  1103,  1106,  1106,  1108,  1116,  1117,  1118,
    1121,  1122,  1125,  1126,  1127,  1127,  1130,  1131,  1134,  1135,
    1136,  1136,  1139,  1140,  1143,  1143,  1146,  1147,  1148,  1151,
    1152,  1153,  1154,  1157,  1164,  1167,  1170,  1173,  1176,  1179,
    1182,  1185,  1188,  1191,  1194,  1197,  1200,  1203,  1206,  1209,
    1212,  1215,  1218,  1221,  1224,  1227,  1230,  1230,  1230,  1231,
    1232,  1232,  1233,  1236,  1237,  1237,  1242,  1245,  1245,  1245,
    1246,  1246,  1246,  1247,  1247,  1247,  1247,  1247,  1247,  1247,
    1247,  1248,  1248,  1248,  1248,  1248,  1249,  1249,  1250,  1253
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DEFINE", "DOMAIN_TOKEN", "PROBLEM",
  "REQUIREMENTS", "TYPES", "CONSTANTS", "PREDICATES", "FUNCTIONS",
  "OBSERVABLES", "STRIPS", "TYPING", "NEGATIVE_PRECONDITIONS",
  "DISJUNCTIVE_PRECONDITIONS", "EQUALITY", "EXISTENTIAL_PRECONDITIONS",
  "UNIVERSAL_PRECONDITIONS", "QUANTIFIED_PRECONDITIONS",
  "CONDITIONAL_EFFECTS", "FLUENTS", "ADL", "DURATIVE_ACTIONS",
  "DURATION_INEQUALITIES", "CONTINUOUS_EFFECTS", "PROBABILISTIC_EFFECTS",
  "REWARDS", "MDP", "ONEOF", "UNKNOWN", "NON_DETERMINISTIC_DYNAMICS",
  "PROBABILISTIC_DYNAMICS", "ACTION", "EVENT", "PARAMETERS",
  "PRECONDITION", "EFFECT", "OBSERVATION", "PDOMAIN", "OBJECTS", "INIT",
  "GOAL", "GOAL_REWARD", "METRIC", "GOAL_PROBABILITY", "WHEN", "NOT",
  "AND", "OR", "IMPLY", "EXISTS", "FORALL", "PROBABILISTIC", "ASSIGN",
  "SCALE_UP", "SCALE_DOWN", "INCREASE", "DECREASE", "MINIMIZE", "MAXIMIZE",
  "NUMBER_TOKEN", "OBJECT_TOKEN", "EITHER", "LE", "GE", "NAME", "VARIABLE",
  "NUMBER", "HORIZON", "DISC", "ILLEGAL_TOKEN", "PLANTIME", "PLAN",
  "FORPROBLEM", "IF", "THEN", "ELSE", "CASE", "GOTO", "DONE",
  "ANTI_COMMENT", "'('", "')'", "'-'", "'='", "'<'", "'>'", "'+'", "'*'",
  "'/'", "$accept", "file", "$@1", "$@2", "$@3", "c_plan", "c_plan_body",
  "c_steps", "c_step", "c_instruction", "c_action", "c_symbols", "c_then",
  "c_else", "c_if", "c_case", "c_guards", "c_goto", "c_label",
  "domains_and_problems", "domain_def", "$@4", "domain_body",
  "domain_body2", "domain_body3", "domain_body4", "domain_body5",
  "domain_body6", "domain_body7", "domain_body8", "domain_body9",
  "observables_def", "$@5", "structure_defs", "structure_def",
  "require_def", "require_keys", "require_key", "types_def", "$@6",
  "constants_def", "$@7", "predicates_def", "functions_def", "$@8",
  "predicate_decls", "predicate_decl", "$@9", "function_decls",
  "function_decl_seq", "function_type_spec", "$@10", "function_decl",
  "$@11", "observable_decls", "observable_decl", "$@12", "action_def",
  "$@13", "$@14", "parameters", "action_body", "action_body2",
  "action_body3", "precondition", "effect", "observations", "eff_formula",
  "$@15", "$@16", "eff_formulas", "prob_effs", "oneof_effs", "or_effs",
  "unknown_effs", "probability", "p_effect", "$@17", "assign_op",
  "observation_defs", "observation", "problem_def", "$@18", "$@19",
  "problem_body", "problem_body_r", "problem_body_ig", "problem_body_h",
  "problem_body_ho", "problem_body_hd", "problem_body_o",
  "problem_body_oh", "problem_body_od", "problem_body_d",
  "problem_body_dh", "problem_body_do", "object_decl", "$@20",
  "horizon_decl", "init", "$@21", "init_elements", "init_element",
  "prob_inits", "oneof_inits", "unknown_inits", "simple_init", "one_inits",
  "one_init", "value", "goal_spec", "discount", "goal_reward",
  "metric_spec", "$@22", "$@23", "formula", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "conjuncts", "disjuncts", "oneof_disjuncts",
  "atomic_term_formula", "$@30", "atomic_name_formula", "$@31",
  "binary_comp", "f_exp", "term_or_f_exp", "$@32", "$@33", "$@34", "$@35",
  "$@36", "opt_f_exp", "f_head", "$@37", "ground_f_exp",
  "opt_ground_f_exp", "ground_f_head", "$@38", "terms", "names",
  "variables", "$@39", "variable_seq", "typed_names", "$@40", "name_seq",
  "type_spec", "$@41", "type", "types", "function_type", "define",
  "domain", "problem", "when", "not", "and", "or", "imply", "exists",
  "forall", "probabilistic", "assign", "scale_up", "scale_down",
  "increase", "decrease", "minimize", "maximize", "number", "object",
  "either", "plantime", "type_name", "predicate", "function", "name",
  "variable", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,    40,    41,    45,    61,    60,    62,    43,    42,
      47
};
# endif

#define YYPACT_NINF (-517)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-12)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -517,    43,  -517,   -36,   -28,  -517,   -14,    69,  -517,  1418,
    -517,    98,  -517,  -517,    31,  -517,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,   478,  1426,  -517,  1455,  -517,
    -517,  -517,    53,    57,  -517,  -517,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  1205,  -517,  -517,  -517,  1455,  -517,  -517,
    1455,  -517,  -517,  -517,  -517,    75,    75,    66,    45,  1636,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,   368,  -517,  -517,  -517,  1280,
      73,   509,  -517,  1455,    78,    95,  -517,   104,   121,   142,
    -517,  -517,  1636,  1636,   159,   613,  -517,  -517,  -517,  -517,
    -517,  -517,  1205,  -517,   602,  1280,  -517,  -517,  -517,  -517,
    -517,   677,   178,   202,   202,   908,  -517,  -517,  -517,  1317,
     189,   192,   198,  -517,  -517,  1280,  1280,  1280,  1280,  -517,
     203,  1280,  1280,  1280,  1280,  -517,   221,  -517,  -517,  -517,
     233,   -26,  -517,   235,  -517,  -517,  -517,  -517,   832,  1463,
    1463,  -517,   238,   261,  1280,  1280,  1280,  1280,   946,  -517,
    1280,  1280,  1280,  1280,  -517,  -517,  1455,  -517,  -517,  -517,
    1455,   278,  1463,  1463,  1674,  -517,  -517,  -517,  -517,   282,
     331,  1636,  -517,   294,   310,   352,   371,  -517,   388,   389,
     390,   391,   984,   394,   205,   202,   395,  -517,  -517,  -517,
    1463,  1492,  1463,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
    1500,  -517,  -517,  1317,  -517,  -517,  -517,  -517,  -517,   226,
     397,  -517,  -517,   320,  -517,   329,   357,   369,   402,   404,
    -517,  1636,   405,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,   424,  -517,  -517,  -517,  -517,  -517,
    -517,   794,  1702,   406,  -517,  -517,  1086,  1354,  1455,   638,
     407,  1094,   409,   870,  1757,  -517,  -517,  -517,  -517,  1636,
    1636,  -517,   212,  -517,   335,  -517,   214,  -517,   373,  -517,
     320,   410,   411,   316,  -517,   412,   320,   413,   415,   124,
    -517,   320,   411,   416,   417,   419,  -517,   381,  -517,  -517,
    -517,  -517,  -517,  1636,  1280,  1463,  -517,  -517,  -517,   202,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
    1641,  -517,  1636,  1636,   167,   420,  -517,  -517,   219,  -517,
     413,   320,   320,   112,  -517,   320,   421,  -517,   295,  -517,
     341,  -517,   168,  -517,   320,   320,  -517,   442,  -517,  -517,
    -517,   730,   432,   251,  -517,  -517,   433,   435,   436,  -517,
    -517,   437,   768,  -517,   440,   638,  -517,  -517,  1636,   448,
      26,  -517,   464,   464,  -517,   320,   229,   320,   320,  -517,
     320,   450,   450,  -517,  1572,   451,   730,  -517,    11,  -517,
    -517,  -517,  -517,  -517,  -517,  -517,  1463,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,   420,  -517,   453,   253,   253,
     320,   217,    19,   463,   465,  -517,   467,   468,  1455,  -517,
    1636,    61,   469,   471,   473,   475,  -517,  -517,  -517,  -517,
     476,  1636,   202,   202,   472,  -517,   202,  1455,  1463,   480,
     484,  -517,  -517,   271,   512,  -517,   491,   638,  -517,  -517,
    1527,   535,  -517,  -517,    96,   257,  -517,   495,   497,   502,
    -517,   517,  -517,  -517,  -517,  -517,  -517,  1636,  -517,  -517,
     503,   504,  -517,  -517,  -517,   506,  -517,  -517,   -19,  -517,
    -517,  -517,  -517,  -517,   328,  -517,  -517,  -517,  1455,   288,
     297,   111,  -517,   298,   314,   319,   518,   519,   511,  1455,
     521,  -517,  -517,  -517,  -517,   315,  -517,   -28,   202,  1535,
    1535,  1391,  -517,    75,  -517,  1124,    84,  -517,  -517,  -517,
    -517,   526,   532,  -517,   533,   534,   339,  -517,   351,   356,
     538,    17,  -517,   351,   527,   541,   -10,  -517,   356,   527,
    -517,   543,   730,   536,  -517,   730,  -517,  -517,  -517,   382,
     529,   400,  1132,  -517,  -517,  -517,   531,  -517,  1636,   547,
    -517,  1158,  1535,   114,  -517,   293,  -517,  -517,   537,   386,
    1243,  1243,   539,   540,  1636,   542,   544,   549,  -517,  -517,
     552,  -517,  -517,  -517,  -517,   584,  -517,  -517,  -517,  -517,
    -517,  -517,   545,   546,   730,   548,  -517,  -517,  1391,  -517,
    -517,  -517,  -517,  -517,   550,  -517,  -517,  -517,  1535,  1022,
     553,    34,   553,  -517,  -517,  -517,  -517,   666,   554,  -517,
     555,  -517,  -517,   556,  -517,  -517,  -517,  -517,   557,  -517,
     562,  1167,  -517,  -517,   561,  -517,  -517,  -517,   426,  -517,
    1243,  -517,  1243,  1243,  1243,  1243,  -517,  -517,  -517,  -517,
    -517,   563,   222,  -517,  -517,  1060,  -517,   583,  1243,  1243,
    1243,  1243,  -517,  -517,   585,  -517,   594,   595,   600,   601,
     588,  -517,  -517,  -517,  -517,  -517
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,    33,     0,     0,     1,     3,     0,     5,     0,
       7,     0,    34,    35,     0,   376,   377,   378,   357,   380,
     381,   385,   384,   379,   382,     0,     0,   259,     0,   383,
     283,   354,     0,     0,   268,   358,   359,   360,   361,   362,
     363,   288,   289,     0,   287,   290,   262,     0,   275,   266,
       0,   271,   273,   281,   172,     0,     0,     0,     0,     0,
     279,   387,   388,   389,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   390,
     391,   392,   408,   409,   297,     0,   260,   308,   309,     0,
       0,     0,   277,     0,     0,     0,   327,     0,     0,     0,
     355,   356,     0,     0,     0,     0,   300,   298,   302,   304,
     306,   386,     0,   291,     0,     0,   296,   314,   264,   265,
     276,     0,     0,   332,   332,     0,   184,   185,   364,     0,
       0,     0,     0,   269,   280,     0,     0,     0,     0,   327,
       0,     0,     0,     0,     0,   312,     0,   267,   278,   270,
       0,   333,   336,     0,   282,   328,   329,   172,     0,     0,
       0,    36,     0,     0,   310,     0,     0,     0,     0,   261,
     310,     0,     0,     0,   327,   263,     0,   344,   334,   337,
       0,     0,     0,     0,     0,   163,   151,   173,   165,    38,
       0,     0,   311,     0,     0,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,   332,     0,   186,   164,   166,
       0,     0,     0,   365,   366,   367,   368,   369,   175,   155,
       0,   161,   153,     0,   177,   178,   179,   180,   181,     0,
       0,    41,    44,    53,    85,    39,    42,    45,    46,    48,
      87,     0,     0,   301,   299,   303,   305,   293,   292,   294,
     295,   313,   272,   373,     0,   345,   346,   347,   335,   274,
     168,     0,     0,     0,   171,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,   112,   117,   115,     0,
       0,    37,     0,    86,     0,    40,     0,    43,     0,    49,
      60,    54,    56,     0,    50,    47,    65,    61,    62,     0,
      52,    71,    66,    67,     0,     0,   374,     0,   158,   167,
     160,   159,   169,     0,     0,     0,   174,   152,   162,   332,
     157,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     102,   103,   104,   105,   106,   107,   108,   109,   101,   100,
       0,    89,   338,   338,     0,   121,   134,   136,     0,    57,
      55,    74,    72,     0,    59,    79,    75,    83,     0,    51,
       0,    63,     0,    64,    82,    80,    69,     0,    70,    68,
     187,     0,     0,     0,   349,   350,     0,     0,     0,    88,
      90,     0,   339,   342,     0,     0,   114,   118,     0,     0,
     122,   124,   138,   138,    58,    73,     0,    76,    77,   130,
      81,     0,     0,    20,     0,     0,     0,    13,     0,    32,
       8,   348,   351,   352,   176,   156,     0,   111,   340,   343,
     113,   119,   128,   116,   126,   121,   125,     0,   145,   145,
      78,     0,     0,     0,     0,   192,   254,     0,     0,    30,
       0,     0,     0,     0,     0,     0,    23,     9,    10,    12,
       0,   338,   332,   332,     0,   123,   332,     0,     0,     0,
       0,   141,   142,   145,   144,   146,     0,     0,    84,   131,
     233,     0,   191,   188,     0,   194,   249,     0,     0,    28,
      31,     0,    18,    14,    15,    16,    17,    21,   154,   341,
       0,     0,   372,   127,   353,     0,   147,   148,     0,   135,
     140,   143,   137,   132,     0,   226,   228,   286,     0,     0,
       0,     0,   193,   205,   198,   212,     0,     0,    26,     0,
       0,    22,   120,   129,   139,     0,   183,     0,   332,     0,
       0,     0,   275,     0,   284,     0,     0,   370,   371,   257,
     255,     0,     0,   219,     0,     0,     0,   196,   208,   210,
       0,     0,   195,   201,   203,     0,     0,   197,   217,   215,
     375,     0,     0,     0,    27,     0,    19,   150,   182,     0,
       0,     0,     0,   237,   239,   243,     0,   238,     0,     0,
     326,     0,     0,     0,   330,     0,   223,   227,     0,   254,
       0,     0,     0,     0,   338,     0,     0,     0,   206,   209,
       0,   207,   211,   199,   202,     0,   200,   204,   214,   218,
     213,   216,     0,     0,     0,     0,   149,   133,     0,   241,
     231,   236,   232,   324,     0,   224,   234,   230,     0,     0,
     254,     0,   254,   246,   252,   321,   315,     0,     0,   320,
       0,   222,   251,     0,   221,   250,   190,    24,     0,    29,
       0,     0,   330,   229,     0,   235,   285,   331,     0,   247,
       0,   248,     0,     0,     0,     0,   258,   256,   220,    25,
     245,     0,     0,   240,   242,     0,   225,     0,   322,     0,
       0,     0,   244,   325,   254,   323,     0,     0,     0,     0,
       0,   253,   317,   316,   318,   319
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -397,  -517,   228,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,   245,  -517,
    -517,  -517,  -517,   452,   457,  -517,   350,  -517,  -274,   387,
     385,  -256,  -517,  -186,  -202,   505,  -517,   362,  -517,  -517,
    -156,  -517,  -195,  -216,  -517,  -517,  -517,  -517,   280,  -517,
    -517,  -517,   313,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
     317,   277,   244,   247,  -517,  -517,  -517,  -143,  -517,  -517,
    -517,   485,  -517,  -517,  -517,   -22,   498,  -517,  -517,   215,
      -3,  -517,  -517,  -517,   342,   308,  -517,  -517,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,  -517,  -360,  -517,  -342,  -517,
    -517,  -517,   195,  -517,  -517,  -517,  -502,  -517,    94,  -517,
    -517,  -458,  -389,  -426,  -517,  -517,    -2,  -517,  -517,  -517,
    -517,  -517,  -517,   216,  -517,  -517,  -146,  -517,  -443,  -517,
    -517,    48,   634,  -517,  -517,  -517,  -517,  -517,   577,   482,
    -517,  -386,  -517,  -516,  -517,  -101,    99,  -116,  -517,  -517,
    -337,  -517,  -517,   370,  -517,  -517,  -517,  -517,  -517,  -517,
    -517,   569,   732,  -179,  -517,  -517,  -517,   574,   -88,  -517,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -287,  -517,  -517,
    -185,   -25,   -82,   -41,   -27
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,     4,     8,   372,   405,   406,   407,
     442,   487,   518,   564,   443,   444,   479,   445,   408,     6,
      12,   189,   230,   231,   232,   289,   294,   300,   349,   354,
     363,   295,   399,   233,   234,   433,   340,   341,   236,   342,
     237,   343,   238,   239,   345,   344,   387,   452,   389,   390,
     425,   454,   391,   453,   431,   469,   528,   240,   392,   393,
     428,   460,   461,   462,   463,   464,   465,   185,   272,   268,
     271,   158,   261,   266,   263,   159,   186,   267,   218,   525,
     526,    13,   401,   402,   434,   435,   512,   552,   603,   606,
     547,   598,   601,   557,   610,   608,   513,   594,   514,   436,
     654,   535,   505,   583,   572,   576,   573,   651,   574,   671,
     475,   515,   633,   634,   591,   590,   120,   112,    89,    92,
      60,    94,    95,    91,   121,   105,    27,    96,   575,   584,
      46,   160,    86,   136,   135,   137,   138,   139,   193,   116,
     174,   638,   686,   639,   652,   125,   629,   150,   205,   151,
     381,   451,   382,   178,   204,   255,   373,   493,    32,   102,
     103,    28,   220,    48,    49,    50,    51,    52,   533,   224,
     225,   226,   227,   228,   539,   540,   494,   256,   307,   561,
      29,    30,   117,   111,   152
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      53,    10,    87,   110,    55,   221,   384,    26,   153,   448,
     476,   129,   403,   187,   187,   579,    88,   188,   104,   257,
     374,   292,   298,   361,    56,   274,    57,   506,   577,    36,
     543,   283,   145,    97,    98,   350,   187,   187,   168,   208,
     209,    83,   291,     5,   303,    90,     7,   369,    93,   100,
     101,   290,   296,   301,     9,   549,   554,   543,   177,   544,
     470,   130,   131,     9,   187,   187,   187,   260,    11,   265,
     621,    87,    14,   202,   270,   352,   394,   660,   509,   298,
     626,   352,   297,   302,   155,    88,   412,   545,   283,   258,
     599,   122,   506,   404,   283,   604,   223,   356,   156,   283,
     397,    31,   650,   134,   545,   351,   355,   356,   388,   296,
     424,   351,   364,    33,   489,   187,   355,   364,   309,   148,
     187,   277,   375,   312,   179,   187,   655,   155,   318,   481,
      59,   632,   276,   277,   352,    58,   182,   115,   508,   297,
     509,   156,   365,    54,   482,   279,   280,   365,    99,   283,
     242,   543,   588,   283,   553,   558,   118,   279,   280,    53,
     123,   155,   283,   146,   351,   613,   395,   589,   615,   187,
     398,   548,   377,   559,   203,   156,   276,   124,   206,   400,
     544,   545,    54,   164,   165,   166,   167,   126,   413,   170,
     171,   172,   173,   283,   607,   128,   283,   627,   283,   611,
     304,   279,   280,   378,   127,   640,   183,   602,    15,    16,
      17,   430,   192,   194,   195,   196,   609,   648,   192,   199,
     200,   201,   276,   277,   278,    15,    16,    17,   283,   278,
     357,   145,   274,   275,   276,   277,   278,    53,   346,   347,
     357,   659,   132,   661,    53,   279,   280,   279,   280,   385,
     386,   182,   279,   280,    15,    16,    17,   643,   691,   279,
     280,   149,   279,   280,    19,    20,   315,   253,    23,    83,
     187,    24,   161,   450,   677,   162,   678,   679,   680,   681,
     163,    19,    20,    21,    22,    23,   169,   254,    24,   457,
     458,   459,   685,   687,   688,   689,    15,    16,    17,   467,
     468,   383,   383,   276,   175,   278,   422,   618,   458,   459,
      19,    20,   187,   253,    23,   497,   176,    24,   180,   527,
     190,   183,   529,   530,   276,   532,   278,   357,   279,   280,
     409,    15,    16,    17,   411,   191,   490,   491,   510,   511,
     495,   419,   275,   276,   277,   278,   128,   537,   538,   279,
     280,   278,    19,    20,    21,    22,    23,   529,   530,    24,
     421,   207,   376,   446,   229,   409,   541,   542,   279,   280,
     241,    61,    62,    63,   279,   280,    36,   243,   531,   510,
     546,   128,   277,   278,    15,    16,    17,    19,    20,    21,
      22,    23,   619,   244,    24,   550,   551,     9,   567,   409,
     555,   556,   282,    15,    16,    17,   279,   280,   544,   545,
     383,   284,   570,   531,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   550,   597,    82,   245,   478,   555,   600,   286,
      19,    20,   503,   253,    23,   507,   521,    24,    36,   580,
     276,   288,   106,   357,   246,   496,   107,   108,   109,    19,
      20,    21,    22,    23,     9,   616,    24,   550,   631,   660,
     509,   247,   248,   249,   250,   279,   280,   252,   259,   534,
     281,    15,    16,    17,   293,   618,   299,   306,   305,   310,
     316,   319,   348,   353,   358,   360,   623,   362,   367,   427,
     370,   371,   388,   396,   507,   507,   536,    34,   580,   580,
     507,   582,    15,    16,    17,   410,   414,   565,   415,   416,
     417,   409,   568,   420,   409,    35,    36,    37,    38,    39,
      40,   423,   432,   492,   447,   456,   580,    19,    20,    21,
      22,    23,    41,    42,    24,   471,   534,   507,   473,   474,
     459,   477,   483,   383,   484,   623,   485,   507,   486,   488,
     534,   628,   498,    43,    44,    45,   568,   499,    19,    20,
      21,    22,    23,   409,   502,    24,   470,   516,   580,   517,
     580,   580,   580,   580,   519,   520,   522,   523,   657,   524,
     560,    25,   119,   563,   592,   562,   580,   580,   580,   580,
     593,   595,   596,   507,   566,    61,    62,    63,   542,   605,
     541,   612,   617,   614,   622,   624,    15,    16,    17,   545,
     630,   544,   641,   642,   543,   644,   507,   645,   646,   647,
     670,   649,   509,   653,   657,   658,   449,   666,   667,   668,
     669,    15,    16,    17,   676,   359,   682,   534,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   684,   690,    82,    61,
      62,    63,    19,    20,    21,    22,    23,   692,   693,    24,
      15,    16,    17,   694,   695,   480,   141,   285,   368,   366,
     142,   143,   144,   287,   235,    25,   133,    19,    20,    21,
      22,    23,   380,   426,    24,   455,   466,   500,   273,   264,
     429,   501,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
     587,   403,    82,    61,    62,    63,    19,    20,    21,    22,
      23,   472,   569,    24,   437,   674,   140,   198,   581,   314,
     662,   675,   418,   219,   663,   664,   665,    47,   222,    25,
     147,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,     0,     0,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,    15,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   -11,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,     0,    82,    61,    62,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,    19,    20,    21,    22,    23,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,   184,   308,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,    82,     0,
     157,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,   114,   181,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,   114,   320,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,     0,    82,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
       0,   154,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       0,     0,    82,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,     0,   197,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,     0,     0,
      82,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,     0,   251,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,    82,    15,
      16,    17,     0,     0,     0,     0,     0,    15,    16,    17,
       0,     0,     0,     0,     0,   656,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,    15,    16,    17,
       0,     0,     0,     0,     0,    15,    16,    17,     0,     0,
       0,     0,     0,   683,     0,    19,    20,    21,    22,    23,
       0,     0,    24,    19,    20,    21,    22,    23,     0,     0,
      24,    15,    16,    17,     0,     0,     0,     0,   184,   311,
      15,    16,    17,     0,     0,     0,   184,   317,     0,     0,
       0,     0,     0,    19,    20,    21,    22,    23,     0,     0,
      24,    19,    20,    21,    22,    23,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,   585,   586,    61,    62,
      63,     0,     0,     0,   571,   620,     0,    19,    20,    21,
      22,    23,     0,     0,    24,     0,    19,    20,    21,    22,
      23,     0,     0,    24,     0,     0,     0,     0,     0,     0,
      25,   625,     0,     0,     0,     0,    61,    62,    63,   672,
     673,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,     0,
       0,    82,    83,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,     0,    85,   635,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,     0,     0,    82,
       0,   636,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,     0,     0,   637,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
       0,     0,   114,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,     0,     0,    82,     0,   157,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     0,     0,   114,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,     0,     0,
      82,    15,    16,    17,     0,     0,     0,     0,     0,    15,
      16,    17,     0,     0,     0,     0,   313,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,     0,     0,    82,    15,    16,
      17,     0,     0,     0,    18,     0,    15,    16,    17,     0,
       0,     0,     0,   578,     0,     0,     0,    19,    20,    21,
      22,    23,     0,     0,    24,    19,    20,    21,    22,    23,
       0,     0,    24,     0,    54,    15,    16,    17,     0,     0,
      25,     0,     0,    15,    16,    17,     0,     0,    25,     0,
       0,     0,     0,     0,    19,    20,    21,    22,    23,     0,
       0,    24,    19,    20,    21,    22,    23,     0,     0,    24,
      15,    16,    17,     0,     0,     0,     0,    25,    15,    16,
      17,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,    19,    20,    21,    22,    23,     0,     0,    24,    19,
      20,    21,    22,    23,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,   262,    61,    62,    63,     0,     0,
       0,     0,   269,     0,     0,     0,    19,    20,    21,    22,
      23,     0,     0,    24,    19,    20,    21,    22,    23,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,   504,
       0,     0,     0,     0,     0,     0,     0,   571,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,    82,    61,
      62,    63,     0,     0,     0,     0,     0,   438,     0,     0,
     439,   440,   441,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
       0,     0,   338,   339,     0,     0,     0,    15,    16,    17,
       0,     0,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       0,     0,    82,   210,   211,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    35,    36,   212,   379,     0,    40,   128,   213,   214,
     215,   216,   217,    19,    20,    21,    22,    23,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,     0,   213,   214,   215,   216,
     217,    19,    20,    21,    22,    23,     0,     0,    24,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,     0,   338,   339
};

static const yytype_int16 yycheck[] =
{
      25,     4,    43,    85,    26,   184,   343,     9,   124,   406,
     436,    99,     1,   159,   160,   531,    43,   160,    59,   204,
     307,   237,   238,   297,    26,     6,    28,   470,   530,    48,
      40,   233,   114,    55,    56,   291,   182,   183,   139,   182,
     183,    67,   237,     0,   239,    47,    82,   303,    50,     4,
       5,   237,   238,   239,    82,   513,   514,    40,    84,    69,
      41,   102,   103,    82,   210,   211,   212,   210,    82,   212,
     572,   112,     3,   174,   220,   291,   350,    43,    44,   295,
     582,   297,   238,   239,   125,   112,   373,    70,   290,   205,
     548,    93,   535,    82,   296,   553,   184,   292,   125,   301,
     356,     3,   618,   105,    70,   291,   292,   302,    82,   295,
      84,   297,   298,    82,   451,   261,   302,   303,   261,   121,
     266,     9,   307,   266,   151,   271,   628,   168,   271,    68,
      73,   589,     8,     9,   350,    82,   158,    89,    42,   295,
      44,   168,   298,    68,    83,    33,    34,   303,    82,   351,
     191,    40,    68,   355,   514,   515,    83,    33,    34,   184,
      82,   202,   364,   115,   350,   562,   352,    83,   565,   315,
     356,   513,   315,   515,   176,   202,     8,    82,   180,   365,
      69,    70,    68,   135,   136,   137,   138,    83,   373,   141,
     142,   143,   144,   395,   554,    53,   398,    83,   400,   559,
     241,    33,    34,   319,    83,   591,   158,   549,     3,     4,
       5,   397,   164,   165,   166,   167,   558,   614,   170,   171,
     172,   173,     8,     9,    10,     3,     4,     5,   430,    10,
      11,   313,     6,     7,     8,     9,    10,   262,   279,   280,
      11,   630,    83,   632,   269,    33,    34,    33,    34,    82,
      83,   273,    33,    34,     3,     4,     5,   594,   684,    33,
      34,    83,    33,    34,    59,    60,   268,    62,    63,    67,
     416,    66,    83,   416,   660,    83,   662,   663,   664,   665,
      82,    59,    60,    61,    62,    63,    83,    82,    66,    36,
      37,    38,   678,   679,   680,   681,     3,     4,     5,    82,
      83,   342,   343,     8,    83,    10,   388,    85,    37,    38,
      59,    60,   458,    62,    63,   458,    83,    66,    83,   498,
      82,   273,    29,    30,     8,   504,    10,    11,    33,    34,
     371,     3,     4,     5,    83,    74,   452,   453,    81,    82,
     456,   382,     7,     8,     9,    10,    53,    59,    60,    33,
      34,    10,    59,    60,    61,    62,    63,    29,    30,    66,
     385,    83,   314,   404,    82,   406,    69,    70,    33,    34,
      39,     3,     4,     5,    33,    34,    48,    83,    85,    81,
      82,    53,     9,    10,     3,     4,     5,    59,    60,    61,
      62,    63,   571,    83,    66,    81,    82,    82,    83,   440,
      81,    82,    82,     3,     4,     5,    33,    34,    69,    70,
     451,    82,   528,    85,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    81,    82,    66,    83,   438,    81,    82,    82,
      59,    60,   467,    62,    63,   470,   487,    66,    48,   531,
       8,    82,    84,    11,    83,   457,    88,    89,    90,    59,
      60,    61,    62,    63,    82,    83,    66,    81,    82,    43,
      44,    83,    83,    83,    83,    33,    34,    83,    83,   504,
      83,     3,     4,     5,    82,    85,    82,    63,    83,    83,
      83,    82,    82,    82,    82,    82,   578,    82,    82,    35,
      83,    82,    82,    82,   529,   530,   508,    29,   590,   591,
     535,   533,     3,     4,     5,    83,    83,   519,    83,    83,
      83,   562,   525,    83,   565,    47,    48,    49,    50,    51,
      52,    83,    82,    61,    83,    82,   618,    59,    60,    61,
      62,    63,    64,    65,    66,    82,   571,   572,    83,    82,
      38,    83,    83,   594,    83,   637,    83,   582,    83,    83,
     585,   583,    82,    85,    86,    87,   569,    83,    59,    60,
      61,    62,    63,   614,    83,    66,    41,    82,   660,    82,
     662,   663,   664,   665,    82,    68,    83,    83,   629,    83,
      72,    82,    83,    82,    68,    76,   678,   679,   680,   681,
      68,    68,    68,   628,    83,     3,     4,     5,    70,    82,
      69,    68,    83,    77,    83,    68,     3,     4,     5,    70,
      83,    69,    83,    83,    40,    83,   651,    83,    83,    83,
      68,    83,    44,    83,   675,    82,   408,    83,    83,    83,
      83,     3,     4,     5,    83,   295,    83,   672,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    83,    82,    66,     3,
       4,     5,    59,    60,    61,    62,    63,    83,    83,    66,
       3,     4,     5,    83,    83,   440,    84,   235,   303,   302,
      88,    89,    90,   236,   189,    82,    83,    59,    60,    61,
      62,    63,   340,   390,    66,   425,   429,   463,   223,   211,
     393,   464,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
     535,     1,    66,     3,     4,     5,    59,    60,    61,    62,
      63,   433,   527,    66,   402,   651,   112,   170,   532,   267,
      84,   652,   382,   184,    88,    89,    90,    25,   184,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    66,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    -1,    66,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    59,    60,    61,    62,    63,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    82,    83,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    66,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    82,    83,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    82,    83,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,    83,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,    83,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,    83,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    66,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,    83,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    66,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    59,    60,    61,    62,    63,
      -1,    -1,    66,    59,    60,    61,    62,    63,    -1,    -1,
      66,     3,     4,     5,    -1,    -1,    -1,    -1,    82,    83,
       3,     4,     5,    -1,    -1,    -1,    82,    83,    -1,    -1,
      -1,    -1,    -1,    59,    60,    61,    62,    63,    -1,    -1,
      66,    59,    60,    61,    62,    63,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    83,     3,     4,
       5,    -1,    -1,    -1,    82,    83,    -1,    59,    60,    61,
      62,    63,    -1,    -1,    66,    -1,    59,    60,    61,    62,
      63,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    83,    -1,    -1,    -1,    -1,     3,     4,     5,    82,
      83,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,    82,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,    -1,    82,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    66,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,    -1,    82,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,    82,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    -1,
      66,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,    -1,    -1,    -1,    82,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    66,     3,     4,
       5,    -1,    -1,    -1,    46,    -1,     3,     4,     5,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    59,    60,    61,
      62,    63,    -1,    -1,    66,    59,    60,    61,    62,    63,
      -1,    -1,    66,    -1,    68,     3,     4,     5,    -1,    -1,
      82,    -1,    -1,     3,     4,     5,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    59,    60,    61,    62,    63,    -1,
      -1,    66,    59,    60,    61,    62,    63,    -1,    -1,    66,
       3,     4,     5,    -1,    -1,    -1,    -1,    82,     3,     4,
       5,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    62,    63,    -1,    -1,    66,    59,
      60,    61,    62,    63,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,     3,     4,     5,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    59,    60,    61,    62,
      63,    -1,    -1,    66,    59,    60,    61,    62,    63,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    66,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      78,    79,    80,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    -1,    -1,    -1,     3,     4,     5,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      -1,    -1,    66,    29,    30,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    83,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    66,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    92,    93,    94,    95,     0,   110,    82,    96,    82,
     171,    82,   111,   172,     3,     3,     4,     5,    46,    59,
      60,    61,    62,    63,    66,    82,   207,   217,   252,   271,
     272,     3,   249,    82,    29,    47,    48,    49,    50,    51,
      52,    64,    65,    85,    86,    87,   221,   253,   254,   255,
     256,   257,   258,   272,    68,   166,   207,   207,    82,    73,
     211,     3,     4,     5,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    66,    67,    68,    82,   223,   274,   275,   209,
     207,   214,   210,   207,   212,   213,   218,   166,   166,    82,
       4,     5,   250,   251,   274,   216,    84,    88,    89,    90,
     273,   274,   208,    68,    82,   222,   230,   273,    83,    83,
     207,   215,   207,    82,    82,   236,    83,    83,    53,   259,
     274,   274,    83,    83,   207,   225,   224,   226,   227,   228,
     223,    84,    88,    89,    90,   273,   222,    83,   207,    83,
     238,   240,   275,   238,    83,   274,   275,    68,   162,   166,
     222,    83,    83,    82,   222,   222,   222,   222,   236,    83,
     222,   222,   222,   222,   231,    83,    83,    84,   244,   275,
      83,    83,   166,   222,    82,   158,   167,   217,   158,   112,
      82,    74,   222,   229,   222,   222,   222,    83,   229,   222,
     222,   222,   236,   207,   245,   239,   207,    83,   158,   158,
      29,    30,    49,    54,    55,    56,    57,    58,   169,   252,
     253,   254,   258,   259,   260,   261,   262,   263,   264,    82,
     113,   114,   115,   124,   125,   126,   129,   131,   133,   134,
     148,    39,   274,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    62,    82,   246,   268,   271,   238,    83,
     158,   163,    82,   165,   167,   158,   164,   168,   160,    82,
     217,   161,   159,   162,     6,     7,     8,     9,    10,    33,
      34,    83,    82,   125,    82,   114,    82,   115,    82,   116,
     124,   133,   134,    82,   117,   122,   124,   131,   134,    82,
     118,   124,   131,   133,   274,    83,    63,   269,    83,   158,
      83,    83,   158,    82,   230,   207,    83,    83,   158,    82,
      83,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    31,    32,
     127,   128,   130,   132,   136,   135,   274,   274,    82,   119,
     122,   124,   134,    82,   120,   124,   133,    11,    82,   117,
      82,   119,    82,   121,   124,   131,   120,    82,   121,   122,
      83,    82,    97,   247,   268,   271,   222,   158,   238,    83,
     128,   241,   243,   274,   241,    82,    83,   137,    82,   139,
     140,   143,   149,   150,   119,   124,    82,   122,   124,   123,
     124,   173,   174,     1,    82,    98,    99,   100,   109,   274,
      83,    83,   268,   271,    83,    83,    83,    83,   244,   274,
      83,   272,   273,    83,    84,   141,   143,    35,   151,   151,
     124,   145,    82,   126,   175,   176,   190,   175,    75,    78,
      79,    80,   101,   105,   106,   108,   274,    83,    98,   100,
     158,   242,   138,   144,   142,   139,    82,    36,    37,    38,
     152,   153,   154,   155,   156,   157,   152,    82,    83,   146,
      41,    82,   176,    83,    82,   201,   204,    83,   207,   107,
     109,    68,    83,    83,    83,    83,    83,   102,    83,   241,
     238,   238,    61,   248,   267,   238,   207,   158,    82,    83,
     153,   154,    83,   272,    82,   193,   219,   272,    42,    44,
      81,    82,   177,   187,   189,   202,    82,    82,   103,    82,
      68,   274,    83,    83,    83,   170,   171,   254,   147,    29,
      30,    85,   254,   259,   272,   192,   207,    59,    60,   265,
     266,    69,    70,    40,    69,    70,    82,   181,   189,   202,
      81,    82,   178,   187,   202,    81,    82,   184,   187,   189,
      72,   270,    76,    82,   104,   207,    83,    83,   171,   170,
     238,    82,   195,   197,   199,   219,   196,   197,    82,   234,
     273,   214,   166,   194,   220,    82,    83,   193,    68,    83,
     206,   205,    68,    68,   188,    68,    68,    82,   182,   202,
      82,   183,   189,   179,   202,    82,   180,   187,   186,   189,
     185,   187,    68,    98,    77,    98,    83,    83,    85,   254,
      83,   197,    83,   273,    68,    83,   197,    83,   166,   237,
      83,    82,   202,   203,   204,    45,    68,    82,   232,   234,
     232,    83,    83,   241,    83,    83,    83,    83,    98,    83,
     234,   198,   235,    83,   191,   197,    83,   274,    82,   203,
      43,   203,    84,    88,    89,    90,    83,    83,    83,    83,
      68,   200,    82,    83,   199,   237,    83,   232,   232,   232,
     232,   232,    83,    83,    83,   232,   233,   232,   232,   232,
      82,   204,    83,    83,    83,    83
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,    91,    93,    92,    94,    92,    95,    92,    96,    97,
      98,    98,    99,    99,   100,   100,   100,   100,   100,   100,
     100,   101,   102,   102,   103,   104,   105,   105,   106,   107,
     107,   108,   109,   110,   110,   110,   112,   111,   113,   113,
     113,   113,   114,   114,   114,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   116,   116,   116,   116,   116,   116,
     116,   117,   117,   117,   117,   117,   118,   118,   118,   118,
     118,   118,   119,   119,   119,   120,   120,   120,   120,   120,
     121,   121,   121,   123,   122,   124,   124,   125,   126,   127,
     127,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     130,   129,   132,   131,   133,   135,   134,   136,   136,   138,
     137,   139,   139,   139,   140,   140,   142,   141,   144,   143,
     145,   145,   147,   146,   149,   148,   150,   148,   151,   151,
     152,   152,   153,   153,   153,   154,   154,   155,   156,   157,
     157,   158,   158,   159,   158,   160,   158,   158,   158,   158,
     158,   161,   161,   162,   162,   162,   162,   163,   163,   164,
     164,   165,   166,   167,   167,   168,   167,   169,   169,   169,
     169,   169,   170,   170,   171,   171,   171,   173,   172,   174,
     172,   175,   175,   176,   177,   177,   177,   177,   178,   178,
     178,   179,   179,   180,   180,   181,   181,   181,   182,   182,
     183,   183,   184,   184,   184,   185,   185,   186,   186,   188,
     187,   189,   189,   190,   191,   190,   192,   192,   193,   193,
     193,   193,   193,   193,   194,   194,   195,   195,   196,   197,
     197,   198,   198,   199,   199,   200,   201,   201,   201,   201,
     202,   202,   203,   203,   204,   205,   204,   206,   204,   207,
     208,   207,   209,   207,   207,   207,   210,   207,   211,   207,
     207,   212,   207,   213,   207,   214,   214,   215,   215,   216,
     216,   218,   217,   217,   220,   219,   219,   221,   221,   221,
     221,   222,   222,   222,   222,   222,   222,   223,   224,   223,
     225,   223,   226,   223,   227,   223,   228,   223,   223,   223,
     229,   229,   231,   230,   230,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   235,   234,   234,   236,   236,   236,
     237,   237,   238,   238,   239,   238,   240,   240,   241,   241,
     242,   241,   243,   243,   245,   244,   246,   246,   246,   247,
     247,   247,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   271,   271,   271,
     271,   271,   271,   272,   272,   272,   273,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   275
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     0,     2,     0,     2,    12,     3,
       2,     0,     2,     1,     3,     3,     3,     3,     3,     5,
       1,     2,     2,     0,     4,     4,     3,     4,     2,     5,
       0,     2,     1,     0,     2,     2,     0,     9,     0,     1,
       2,     1,     1,     2,     1,     1,     1,     2,     1,     2,
       2,     3,     2,     1,     1,     2,     1,     2,     3,     2,
       1,     1,     1,     2,     2,     1,     1,     1,     2,     2,
       2,     1,     1,     2,     1,     1,     2,     2,     3,     1,
       1,     2,     1,     0,     5,     1,     2,     1,     4,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     5,     0,     5,     4,     0,     5,     0,     2,     0,
       5,     0,     1,     3,     1,     2,     0,     3,     0,     5,
       0,     2,     0,     5,     0,     7,     0,     7,     0,     4,
       2,     1,     1,     2,     1,     0,     1,     2,     2,     5,
       4,     1,     4,     0,     8,     0,     6,     4,     4,     4,
       4,     0,     2,     2,     3,     2,     3,     2,     1,     2,
       1,     1,     1,     1,     4,     0,     6,     1,     1,     1,
       1,     1,     2,     1,     5,     5,     8,     0,    13,     0,
      17,     2,     1,     3,     0,     2,     2,     2,     0,     2,
       2,     0,     1,     0,     1,     0,     2,     2,     0,     1,
       0,     1,     0,     2,     2,     0,     1,     0,     1,     0,
       5,     4,     4,     5,     0,     8,     0,     2,     1,     5,
       4,     4,     4,     0,     2,     3,     2,     1,     1,     1,
       4,     0,     2,     1,     5,     1,     5,     6,     6,     1,
       4,     4,     1,     5,     0,     0,     6,     0,     6,     1,
       0,     6,     0,     6,     4,     4,     0,     5,     0,     5,
       5,     0,     8,     0,     8,     0,     2,     0,     2,     0,
       2,     0,     5,     1,     0,     5,     1,     1,     1,     1,
       1,     1,     5,     5,     5,     5,     1,     1,     0,     6,
       0,     6,     0,     6,     0,     6,     0,     5,     1,     1,
       0,     1,     0,     5,     1,     1,     5,     5,     5,     5,
       1,     1,     0,     1,     0,     5,     1,     0,     2,     2,
       0,     2,     0,     1,     0,     4,     1,     2,     0,     1,
       0,     4,     1,     2,     0,     3,     1,     1,     4,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 356 "parser.yy"
       { success = true; line_number = 1; }
#line 2521 "parser.cc"
    break;

  case 3:
#line 356 "parser.yy"
                                                                 {}
#line 2527 "parser.cc"
    break;

  case 4:
#line 358 "parser.yy"
   { success = true; line_number = 1; }
#line 2533 "parser.cc"
    break;

  case 5:
#line 358 "parser.yy"
                                               {}
#line 2539 "parser.cc"
    break;

  case 6:
#line 359 "parser.yy"
  { success = true; line_number = 1; }
#line 2545 "parser.cc"
    break;

  case 7:
#line 359 "parser.yy"
                                                   {}
#line 2551 "parser.cc"
    break;

  case 8:
#line 374 "parser.yy"
            {the_plan=(yyvsp[-1].t_plan); (yyval.t_plan)->name = (char*)(yyvsp[-7].str)->c_str(); (yyval.t_plan)->problem_name = (char*)(yyvsp[-3].str)->c_str();}
#line 2557 "parser.cc"
    break;

  case 9:
#line 378 "parser.yy"
{
  //cout << "Plan" << endl;
  (yyval.t_plan) = new plan(); 
  (yyval.t_plan)->begin = (yyvsp[-1].t_instr);
}
#line 2567 "parser.cc"
    break;

  case 10:
#line 388 "parser.yy"
        {
	  //cout << "Steps" << endl;
	  (yyval.t_instr)= (yyvsp[-1].t_instr);
	  (yyval.t_instr)->next = (yyvsp[0].t_instr);
	}
#line 2577 "parser.cc"
    break;

  case 11:
#line 393 "parser.yy"
                { (yyval.t_instr)= 0; }
#line 2583 "parser.cc"
    break;

  case 12:
#line 396 "parser.yy"
{
  //cout << "Label Step" << endl;
  // mark label as associated with this instruction
  (yyvsp[-1].t_label_symbol)->instr = (yyvsp[0].t_instr);
  (yyval.t_instr) = (yyvsp[0].t_instr); 
}
#line 2594 "parser.cc"
    break;

  case 13:
#line 403 "parser.yy"
{ 
  //cout << "Step" << endl;
  (yyval.t_instr) = (yyvsp[0].t_instr); 
}
#line 2603 "parser.cc"
    break;

  case 14:
#line 409 "parser.yy"
                                 { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2609 "parser.cc"
    break;

  case 15:
#line 410 "parser.yy"
                       { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2615 "parser.cc"
    break;

  case 16:
#line 411 "parser.yy"
                         { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2621 "parser.cc"
    break;

  case 17:
#line 412 "parser.yy"
                         { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2627 "parser.cc"
    break;

  case 18:
#line 414 "parser.yy"
{ 
  //cout << "Done" << endl;
  (yyval.t_instr) = new Done(); 
}
#line 2636 "parser.cc"
    break;

  case 19:
#line 419 "parser.yy"
{
  //cout << "LCPP"<<endl;
  (yyval.t_instr) = new Lcp_Done((yyvsp[-2].num)->double_value(), (yyvsp[-1].num)->double_value());
}
#line 2645 "parser.cc"
    break;

  case 20:
#line 424 "parser.yy"
        {std::cout <<"trouble parsing instruction"<<std::endl;}
#line 2651 "parser.cc"
    break;

  case 21:
#line 428 "parser.yy"
  {
    //cout << "Action" << endl;
    std::string tmp;
    tmp += "(";
    tmp += (std::string)*(yyvsp[-1].str);
    std::vector<std::string*>::iterator c;
    for(c=((std::vector<std::string*>*)((yyvsp[0].strs)))->begin(); c!=((std::vector<std::string*>*)((yyvsp[0].strs)))->end(); c++)
      {
	tmp += " ";
	tmp += (**c);
      }
    tmp += ")";

    (yyval.t_instr) = new mAction(findAct(tmp));
  }
#line 2671 "parser.cc"
    break;

  case 22:
#line 446 "parser.yy"
                  {(yyval.strs)=(yyvsp[-1].strs); (yyvsp[-1].strs)->push_back((yyvsp[0].str));}
#line 2677 "parser.cc"
    break;

  case 23:
#line 447 "parser.yy"
              {(yyval.strs)=new   std::vector<const std::string*>;}
#line 2683 "parser.cc"
    break;

  case 24:
#line 451 "parser.yy"
{
  //cout << "Then" << endl;
  (yyval.t_instr) = (yyvsp[-1].t_instr);
}
#line 2692 "parser.cc"
    break;

  case 25:
#line 458 "parser.yy"
{
  //cout << "Else" << endl;
  (yyval.t_instr) = (yyvsp[-1].t_instr);
}
#line 2701 "parser.cc"
    break;

  case 26:
#line 465 "parser.yy"
{
  //cout << "IfThen" << endl;
  (yyval.t_instr) = new IfThen(formula_bdd(*(yyvsp[-1].formula), false),(yyvsp[0].t_instr));
}
#line 2710 "parser.cc"
    break;

  case 27:
#line 470 "parser.yy"
{
  //cout << "IfThenElse" << endl;
  (yyval.t_instr) = new IfThenElse(formula_bdd(*(yyvsp[-2].formula), false),(yyvsp[-1].t_instr),(yyvsp[0].t_instr));
}
#line 2719 "parser.cc"
    break;

  case 28:
#line 477 "parser.yy"
{
  //cout << "Case" << endl;
  (yyval.t_instr) = new Case((yyvsp[0].t_guards));
}
#line 2728 "parser.cc"
    break;

  case 29:
#line 484 "parser.yy"
{
  //cout << "Guards" << endl;
  (yyval.t_guards) = (yyvsp[-4].t_guards);
  (yyval.t_guards)->push_back(std::make_pair(formula_bdd(*(yyvsp[-2].formula), false),(yyvsp[-1].t_instr)));
}
#line 2738 "parser.cc"
    break;

  case 30:
#line 489 "parser.yy"
              { (yyval.t_guards) = new Guards(); }
#line 2744 "parser.cc"
    break;

  case 31:
#line 494 "parser.yy"
{
  //cout << "Goto" << endl;
  (yyval.t_instr) = new Goto((yyvsp[0].t_label_symbol));
}
#line 2753 "parser.cc"
    break;

  case 32:
#line 501 "parser.yy"
{
  // make a label object
  // put/find in a hash table
  (yyval.t_label_symbol) = new label_symbol(*(yyvsp[0].str));//current_analysis->label_tab.symbol_ref($1);
}
#line 2763 "parser.cc"
    break;

  case 36:
#line 517 "parser.yy"
                                            { make_domain((yyvsp[-1].str)); }
#line 2769 "parser.cc"
    break;

  case 83:
#line 584 "parser.yy"
                                   {// cout << "ob tokens"<<endl;
}
#line 2776 "parser.cc"
    break;

  case 84:
#line 585 "parser.yy"
                       {// cout << "done ob tokens"<<endl;
}
#line 2783 "parser.cc"
    break;

  case 91:
#line 603 "parser.yy"
                     { requirements->strips = true; }
#line 2789 "parser.cc"
    break;

  case 92:
#line 604 "parser.yy"
                     { requirements->typing = true; }
#line 2795 "parser.cc"
    break;

  case 93:
#line 606 "parser.yy"
                { requirements->negative_preconditions = true; }
#line 2801 "parser.cc"
    break;

  case 94:
#line 608 "parser.yy"
                { requirements->disjunctive_preconditions = true; }
#line 2807 "parser.cc"
    break;

  case 95:
#line 609 "parser.yy"
                       { requirements->equality = true; }
#line 2813 "parser.cc"
    break;

  case 96:
#line 611 "parser.yy"
                { requirements->existential_preconditions = true; }
#line 2819 "parser.cc"
    break;

  case 97:
#line 613 "parser.yy"
                { requirements->universal_preconditions = true; }
#line 2825 "parser.cc"
    break;

  case 98:
#line 615 "parser.yy"
                { requirements->quantified_preconditions(); }
#line 2831 "parser.cc"
    break;

  case 99:
#line 616 "parser.yy"
                                  { requirements->conditional_effects = true; }
#line 2837 "parser.cc"
    break;

  case 100:
#line 617 "parser.yy"
                                     { requirements->probabilistic = true; }
#line 2843 "parser.cc"
    break;

  case 101:
#line 618 "parser.yy"
                                         { requirements->non_deterministic = true; 
	    requirements->probabilistic_effects = true; 	    requirements->probabilistic = false;}
#line 2850 "parser.cc"
    break;

  case 102:
#line 620 "parser.yy"
                      { requirements->fluents = true; }
#line 2856 "parser.cc"
    break;

  case 103:
#line 621 "parser.yy"
                  { requirements->adl(); }
#line 2862 "parser.cc"
    break;

  case 104:
#line 623 "parser.yy"
                { throw Exception("`:durative-actions' not supported"); }
#line 2868 "parser.cc"
    break;

  case 105:
#line 625 "parser.yy"
                { throw Exception("`:duration-inequalities' not supported"); }
#line 2874 "parser.cc"
    break;

  case 106:
#line 627 "parser.yy"
                { throw Exception("`:continuous-effects' not supported"); }
#line 2880 "parser.cc"
    break;

  case 107:
#line 629 "parser.yy"
                {
		  requirements->probabilistic_effects = true;
		  requirements->probabilistic = true;
		  requirements->non_deterministic = false;
		  goal_prob_function =
		    domain->functions().add_function("goal-probability");
		}
#line 2892 "parser.cc"
    break;

  case 108:
#line 637 "parser.yy"
                {
		  requirements->rewards = true;
		  reward_function = domain->functions().add_function("reward");
		}
#line 2901 "parser.cc"
    break;

  case 109:
#line 642 "parser.yy"
                {
		  requirements->probabilistic_effects = true;
		  requirements->rewards = true;
		  goal_prob_function =
		    domain->functions().add_function("goal-probability");
		  reward_function = domain->functions().add_function("reward");
		}
#line 2913 "parser.cc"
    break;

  case 110:
#line 651 "parser.yy"
                      { require_typing(); name_kind = TYPE_KIND; }
#line 2919 "parser.cc"
    break;

  case 111:
#line 652 "parser.yy"
                              { name_kind = VOID_KIND; }
#line 2925 "parser.cc"
    break;

  case 112:
#line 655 "parser.yy"
                              { name_kind = CONSTANT_KIND; }
#line 2931 "parser.cc"
    break;

  case 113:
#line 656 "parser.yy"
                  { name_kind = VOID_KIND; }
#line 2937 "parser.cc"
    break;

  case 115:
#line 662 "parser.yy"
                              { require_fluents(); }
#line 2943 "parser.cc"
    break;

  case 119:
#line 673 "parser.yy"
                               { make_predicate((yyvsp[0].str)); }
#line 2949 "parser.cc"
    break;

  case 120:
#line 674 "parser.yy"
                   { parsing_predicate = false; }
#line 2955 "parser.cc"
    break;

  case 126:
#line 686 "parser.yy"
                         { require_typing(); }
#line 2961 "parser.cc"
    break;

  case 128:
#line 689 "parser.yy"
                             { make_function((yyvsp[0].str)); }
#line 2967 "parser.cc"
    break;

  case 129:
#line 690 "parser.yy"
                  { parsing_function = false; }
#line 2973 "parser.cc"
    break;

  case 132:
#line 697 "parser.yy"
                                 {make_predicate((yyvsp[0].str));// make_observable($2); parsing_obs_token = false;
}
#line 2980 "parser.cc"
    break;

  case 133:
#line 698 "parser.yy"
                 { //parsing_obs_token = false;
 parsing_predicate = false; }
#line 2987 "parser.cc"
    break;

  case 134:
#line 706 "parser.yy"
                             {// cout << *$3<<endl;
               make_action((yyvsp[0].str));  }
#line 2994 "parser.cc"
    break;

  case 135:
#line 708 "parser.yy"
                                          { add_action(); }
#line 3000 "parser.cc"
    break;

  case 136:
#line 709 "parser.yy"
                            {
               make_action((yyvsp[0].str));  }
#line 3007 "parser.cc"
    break;

  case 137:
#line 711 "parser.yy"
                                          { add_event(); }
#line 3013 "parser.cc"
    break;

  case 147:
#line 732 "parser.yy"
                                    { action->set_precondition(*(yyvsp[0].formula)); }
#line 3019 "parser.cc"
    break;

  case 148:
#line 735 "parser.yy"
                            { action->set_effect(*(yyvsp[0].effect)); }
#line 3025 "parser.cc"
    break;

  case 149:
#line 738 "parser.yy"
                                                         {action->set_observation(*(yyvsp[-1].observation_defs));}
#line 3031 "parser.cc"
    break;

  case 150:
#line 739 "parser.yy"
                                                     {action->set_observation(*(yyvsp[-1].observation_defs)); // cout << "parse ob"<<endl;
}
#line 3038 "parser.cc"
    break;

  case 152:
#line 746 "parser.yy"
                                       { (yyval.effect) = (yyvsp[-1].ceffect); }
#line 3044 "parser.cc"
    break;

  case 153:
#line 747 "parser.yy"
                         { prepare_forall_effect(); }
#line 3050 "parser.cc"
    break;

  case 154:
#line 748 "parser.yy"
                                { (yyval.effect) = make_forall_effect(*(yyvsp[-1].effect)); }
#line 3056 "parser.cc"
    break;

  case 155:
#line 749 "parser.yy"
           { require_conditional_effects(); }
#line 3062 "parser.cc"
    break;

  case 156:
#line 750 "parser.yy"
                                { (yyval.effect) = &ConditionalEffect::make(*(yyvsp[-2].formula), *(yyvsp[-1].effect)); }
#line 3068 "parser.cc"
    break;

  case 157:
#line 751 "parser.yy"
                                              { if((yyvsp[-1].peffect)->size() ==1 && (yyvsp[-1].peffect)->probability(0) == 1.0){ (yyval.effect)=&(yyvsp[-1].peffect)->effect(0);} else (yyval.effect) = (yyvsp[-1].peffect); }
#line 3074 "parser.cc"
    break;

  case 158:
#line 752 "parser.yy"
                                       { (yyval.effect) = (yyvsp[-1].peffect); }
#line 3080 "parser.cc"
    break;

  case 159:
#line 753 "parser.yy"
                                 { (yyval.effect) = (yyvsp[-1].peffect); }
#line 3086 "parser.cc"
    break;

  case 160:
#line 754 "parser.yy"
                                           { (yyval.effect) = (yyvsp[-1].peffect); }
#line 3092 "parser.cc"
    break;

  case 161:
#line 757 "parser.yy"
                           { (yyval.ceffect) = new ConjunctiveEffect(); }
#line 3098 "parser.cc"
    break;

  case 162:
#line 758 "parser.yy"
                                        { (yyval.ceffect) = (yyvsp[-1].ceffect); (yyval.ceffect)->add_conjunct(*(yyvsp[0].effect)); }
#line 3104 "parser.cc"
    break;

  case 163:
#line 762 "parser.yy"
               {
 		(yyval.peffect) = new ProbabilisticEffect();
 		add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect));
 	      }
#line 3113 "parser.cc"
    break;

  case 164:
#line 767 "parser.yy"
              { (yyval.peffect) = (yyvsp[-2].peffect); add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect)); }
#line 3119 "parser.cc"
    break;

  case 165:
#line 769 "parser.yy"
              {
                //$1->print(cout,problem->domain().functions(),problem->terms()); 
		(yyval.peffect) = new ProbabilisticEffect();
		add_feffect_outcome(*(yyval.peffect), (yyvsp[-1].expr), *(yyvsp[0].effect));
	      }
#line 3129 "parser.cc"
    break;

  case 166:
#line 776 "parser.yy"
          {
	    //$2->print(cout,problem->domain().functions(),problem->terms()); 
	   (yyval.peffect) = (yyvsp[-2].peffect); add_feffect_outcome(*(yyval.peffect), (yyvsp[-1].expr), *(yyvsp[0].effect)); }
#line 3137 "parser.cc"
    break;

  case 167:
#line 782 "parser.yy"
             { (yyval.peffect) = (yyvsp[-1].peffect); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3143 "parser.cc"
    break;

  case 168:
#line 784 "parser.yy"
{ (yyval.peffect) = new ProbabilisticEffect(); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3149 "parser.cc"
    break;

  case 169:
#line 788 "parser.yy"
             { (yyval.peffect) = (yyvsp[-1].peffect); add_effect_outcome(*(yyval.peffect), new Rational(-3.0), *(yyvsp[0].effect)); }
#line 3155 "parser.cc"
    break;

  case 170:
#line 790 "parser.yy"
{ (yyval.peffect) = new ProbabilisticEffect(); add_effect_outcome(*(yyval.peffect), new Rational(-3.0), *(yyvsp[0].effect)); }
#line 3161 "parser.cc"
    break;

  case 171:
#line 794 "parser.yy"
               { (yyval.peffect) = new ProbabilisticEffect(); 
                 add_effect_outcome(*(yyval.peffect), new Rational(-2.0), *(yyvsp[0].effect)); }
#line 3168 "parser.cc"
    break;

  case 173:
#line 803 "parser.yy"
                               { (yyval.effect) = make_add_effect(*(yyvsp[0].atom)); }
#line 3174 "parser.cc"
    break;

  case 174:
#line 804 "parser.yy"
                                           { (yyval.effect) = make_delete_effect(*(yyvsp[-1].atom)); }
#line 3180 "parser.cc"
    break;

  case 175:
#line 805 "parser.yy"
                         { effect_fluent = true; }
#line 3186 "parser.cc"
    break;

  case 176:
#line 806 "parser.yy"
             { (yyval.effect) = make_assignment_effect((yyvsp[-4].setop), *(yyvsp[-2].appl), *(yyvsp[-1].expr)); }
#line 3192 "parser.cc"
    break;

  case 177:
#line 809 "parser.yy"
                   { (yyval.setop) = Assignment::ASSIGN_OP; }
#line 3198 "parser.cc"
    break;

  case 178:
#line 810 "parser.yy"
                     { (yyval.setop) = Assignment::SCALE_UP_OP; }
#line 3204 "parser.cc"
    break;

  case 179:
#line 811 "parser.yy"
                       { (yyval.setop) = Assignment::SCALE_DOWN_OP; }
#line 3210 "parser.cc"
    break;

  case 180:
#line 812 "parser.yy"
                     { (yyval.setop) = Assignment::INCREASE_OP; }
#line 3216 "parser.cc"
    break;

  case 181:
#line 813 "parser.yy"
                     { (yyval.setop) = Assignment::DECREASE_OP; }
#line 3222 "parser.cc"
    break;

  case 182:
#line 819 "parser.yy"
                                                {(yyval.observation_defs)->add_entry((yyvsp[0].observation)); }
#line 3228 "parser.cc"
    break;

  case 183:
#line 820 "parser.yy"
                               {(yyval.observation_defs) = new Observation(); (yyval.observation_defs)->add_entry((yyvsp[0].observation)); }
#line 3234 "parser.cc"
    break;

  case 184:
#line 823 "parser.yy"
                                                        {(yyval.observation) = make_observation(*(yyvsp[-3].formula), *(yyvsp[-2].num), *(yyvsp[-1].num));}
#line 3240 "parser.cc"
    break;

  case 185:
#line 824 "parser.yy"
                                        {(yyval.observation) = make_observation(*(yyvsp[-3].formula), *(yyvsp[-2].formula) , *(yyvsp[-1].num)); }
#line 3246 "parser.cc"
    break;

  case 186:
#line 825 "parser.yy"
                                                        {(yyval.observation) = make_observation(*(yyvsp[-5].formula), *(yyvsp[-2].peffect)); (yyvsp[-2].peffect)->setObservation();}
#line 3252 "parser.cc"
    break;

  case 187:
#line 842 "parser.yy"
                { make_problem((yyvsp[-5].str), (yyvsp[-1].str)); }
#line 3258 "parser.cc"
    break;

  case 188:
#line 843 "parser.yy"
                { problem->instantiate_actions(); problem->instantiate_events(); delete requirements; }
#line 3264 "parser.cc"
    break;

  case 189:
#line 845 "parser.yy"
               { make_problem((yyvsp[-5].str), (yyvsp[-1].str)); }
#line 3270 "parser.cc"
    break;

  case 190:
#line 846 "parser.yy"
               { problem->instantiate_actions();  problem->instantiate_events(); problem->set_plan_time(*(yyvsp[-1].num));
	         delete (yyvsp[-1].num); delete requirements; }
#line 3277 "parser.cc"
    break;

  case 219:
#line 908 "parser.yy"
                          { name_kind = OBJECT_KIND; }
#line 3283 "parser.cc"
    break;

  case 220:
#line 909 "parser.yy"
                { name_kind = VOID_KIND; }
#line 3289 "parser.cc"
    break;

  case 221:
#line 912 "parser.yy"
                                      {problem->set_plan_horizon(*(yyvsp[-1].num));}
#line 3295 "parser.cc"
    break;

  case 222:
#line 913 "parser.yy"
                                               {problem->set_plan_horizon(*(yyvsp[-1].num));}
#line 3301 "parser.cc"
    break;

  case 224:
#line 919 "parser.yy"
       { problem->set_init_formula(*(yyvsp[-1].conj)); get_init_elts();}
#line 3307 "parser.cc"
    break;

  case 228:
#line 928 "parser.yy"
                                   { problem->add_init_atom(*(yyvsp[0].atom));  problem->add_init_effect(*(new AddEffect(*(yyvsp[0].atom))));}
#line 3313 "parser.cc"
    break;

  case 229:
#line 930 "parser.yy"
                 { problem->add_init_value(*(yyvsp[-2].appl), *(yyvsp[-1].num)); delete (yyvsp[-1].num); }
#line 3319 "parser.cc"
    break;

  case 230:
#line 932 "parser.yy"
                 { problem->add_init_effect(*(yyvsp[-1].peffect)); }
#line 3325 "parser.cc"
    break;

  case 231:
#line 933 "parser.yy"
                                        { problem->add_init_effect(*(yyvsp[-1].peffect)); }
#line 3331 "parser.cc"
    break;

  case 232:
#line 934 "parser.yy"
                                            { problem->add_init_effect(*(yyvsp[-1].peffect)); }
#line 3337 "parser.cc"
    break;

  case 234:
#line 946 "parser.yy"
               {
		 (yyval.peffect) = new ProbabilisticEffect();
		 add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect));
	       }
#line 3346 "parser.cc"
    break;

  case 235:
#line 951 "parser.yy"
           { (yyval.peffect) = (yyvsp[-2].peffect); add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect)); }
#line 3352 "parser.cc"
    break;

  case 236:
#line 955 "parser.yy"
             { (yyval.peffect) = (yyvsp[-1].peffect); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3358 "parser.cc"
    break;

  case 237:
#line 957 "parser.yy"
{ (yyval.peffect) = new ProbabilisticEffect(); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3364 "parser.cc"
    break;

  case 238:
#line 961 "parser.yy"
               { (yyval.peffect) = new ProbabilisticEffect(); 
                 add_effect_outcome(*(yyval.peffect), new Rational(-2.0), *(yyvsp[0].effect)); }
#line 3371 "parser.cc"
    break;

  case 240:
#line 966 "parser.yy"
                                    { (yyval.effect) = (yyvsp[-1].ceffect); }
#line 3377 "parser.cc"
    break;

  case 241:
#line 969 "parser.yy"
                        { (yyval.ceffect) = new ConjunctiveEffect(); }
#line 3383 "parser.cc"
    break;

  case 242:
#line 970 "parser.yy"
                               { (yyval.ceffect) = (yyvsp[-1].ceffect); (yyval.ceffect)->add_conjunct(*(yyvsp[0].effect)); }
#line 3389 "parser.cc"
    break;

  case 243:
#line 973 "parser.yy"
                               { (yyval.effect) = make_add_effect(*(yyvsp[0].atom)); }
#line 3395 "parser.cc"
    break;

  case 244:
#line 975 "parser.yy"
             { (yyval.effect) = make_assignment_effect(Assignment::ASSIGN_OP, *(yyvsp[-2].appl), *(yyvsp[-1].expr)); }
#line 3401 "parser.cc"
    break;

  case 245:
#line 978 "parser.yy"
               { (yyval.expr) = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3407 "parser.cc"
    break;

  case 246:
#line 981 "parser.yy"
                                             { problem->set_goal(*(yyvsp[-2].formula), true); }
#line 3413 "parser.cc"
    break;

  case 247:
#line 982 "parser.yy"
                                                    { problem->set_goal(*(yyvsp[-3].formula), *(yyvsp[-2].num)); delete (yyvsp[-2].num); }
#line 3419 "parser.cc"
    break;

  case 248:
#line 983 "parser.yy"
                                                      { problem->set_goal(*(yyvsp[-3].formula), true); }
#line 3425 "parser.cc"
    break;

  case 250:
#line 987 "parser.yy"
                               {set_discount(*(yyvsp[-1].num));}
#line 3431 "parser.cc"
    break;

  case 251:
#line 988 "parser.yy"
                                        {set_discount(*(yyvsp[-1].num));}
#line 3437 "parser.cc"
    break;

  case 253:
#line 993 "parser.yy"
                { set_goal_reward(*(yyvsp[-2].expr)); }
#line 3443 "parser.cc"
    break;

  case 254:
#line 996 "parser.yy"
                          { set_default_metric(); }
#line 3449 "parser.cc"
    break;

  case 255:
#line 997 "parser.yy"
                                  { metric_fluent = true; }
#line 3455 "parser.cc"
    break;

  case 256:
#line 998 "parser.yy"
                { problem->set_metric(*(yyvsp[-1].expr)); metric_fluent = false; }
#line 3461 "parser.cc"
    break;

  case 257:
#line 999 "parser.yy"
                                  { metric_fluent = true; }
#line 3467 "parser.cc"
    break;

  case 258:
#line 1000 "parser.yy"
                { problem->set_metric(*(yyvsp[-1].expr), true); metric_fluent = false; }
#line 3473 "parser.cc"
    break;

  case 259:
#line 1007 "parser.yy"
                              { (yyval.formula) = (yyvsp[0].atom); }
#line 3479 "parser.cc"
    break;

  case 260:
#line 1009 "parser.yy"
            { first_eq_term = eq_term; first_eq_expr = eq_expr; }
#line 3485 "parser.cc"
    break;

  case 261:
#line 1010 "parser.yy"
                              { (yyval.formula) = make_equality(); }
#line 3491 "parser.cc"
    break;

  case 262:
#line 1011 "parser.yy"
                          { require_fluents(); }
#line 3497 "parser.cc"
    break;

  case 263:
#line 1012 "parser.yy"
            { (yyval.formula) = new Comparison((yyvsp[-4].comp), *(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3503 "parser.cc"
    break;

  case 264:
#line 1013 "parser.yy"
                              { (yyval.formula) = make_negation(*(yyvsp[-1].formula)); }
#line 3509 "parser.cc"
    break;

  case 265:
#line 1014 "parser.yy"
                                { (yyval.formula) = (yyvsp[-1].conj); }
#line 3515 "parser.cc"
    break;

  case 266:
#line 1015 "parser.yy"
                 { require_disjunction(); }
#line 3521 "parser.cc"
    break;

  case 267:
#line 1015 "parser.yy"
                                                          { (yyval.formula) = (yyvsp[-1].disj); }
#line 3527 "parser.cc"
    break;

  case 268:
#line 1016 "parser.yy"
                    { require_disjunction(); }
#line 3533 "parser.cc"
    break;

  case 269:
#line 1016 "parser.yy"
                                                                   { (yyval.formula) = (yyvsp[-1].odisj); }
#line 3539 "parser.cc"
    break;

  case 270:
#line 1017 "parser.yy"
                                        { (yyval.formula) = make_implication(*(yyvsp[-2].formula), *(yyvsp[-1].formula)); }
#line 3545 "parser.cc"
    break;

  case 271:
#line 1018 "parser.yy"
                     { prepare_exists(); }
#line 3551 "parser.cc"
    break;

  case 272:
#line 1019 "parser.yy"
            { (yyval.formula) = make_exists(*(yyvsp[-1].formula)); }
#line 3557 "parser.cc"
    break;

  case 273:
#line 1020 "parser.yy"
                     { prepare_forall(); }
#line 3563 "parser.cc"
    break;

  case 274:
#line 1021 "parser.yy"
            { (yyval.formula) = make_forall(*(yyvsp[-1].formula)); }
#line 3569 "parser.cc"
    break;

  case 275:
#line 1024 "parser.yy"
                        { (yyval.conj) = new Conjunction(); }
#line 3575 "parser.cc"
    break;

  case 276:
#line 1025 "parser.yy"
                              { (yyval.conj)->add_conjunct(*(yyvsp[0].formula)); }
#line 3581 "parser.cc"
    break;

  case 277:
#line 1028 "parser.yy"
                        { (yyval.disj) = new Disjunction(); }
#line 3587 "parser.cc"
    break;

  case 278:
#line 1029 "parser.yy"
                              { (yyval.disj)->add_disjunct(*(yyvsp[0].formula)); }
#line 3593 "parser.cc"
    break;

  case 279:
#line 1031 "parser.yy"
                              { (yyval.odisj) = new OneOfDisjunction(); }
#line 3599 "parser.cc"
    break;

  case 280:
#line 1032 "parser.yy"
                                    { (yyval.odisj)->add_oneof_disjunct(*(yyvsp[0].formula)); }
#line 3605 "parser.cc"
    break;

  case 281:
#line 1035 "parser.yy"
                                    { prepare_atom((yyvsp[0].str)); }
#line 3611 "parser.cc"
    break;

  case 282:
#line 1036 "parser.yy"
                        { (yyval.atom) = make_atom(); }
#line 3617 "parser.cc"
    break;

  case 283:
#line 1037 "parser.yy"
                                { prepare_atom((yyvsp[0].str)); (yyval.atom) = make_atom(); }
#line 3623 "parser.cc"
    break;

  case 284:
#line 1040 "parser.yy"
                                    { prepare_atom((yyvsp[0].str)); }
#line 3629 "parser.cc"
    break;

  case 285:
#line 1041 "parser.yy"
                        { (yyval.atom) = make_atom(); }
#line 3635 "parser.cc"
    break;

  case 286:
#line 1042 "parser.yy"
                                { prepare_atom((yyvsp[0].str)); (yyval.atom) = make_atom(); }
#line 3641 "parser.cc"
    break;

  case 287:
#line 1045 "parser.yy"
                  { (yyval.comp) = Comparison::LT_CMP; }
#line 3647 "parser.cc"
    break;

  case 288:
#line 1046 "parser.yy"
                 { (yyval.comp) = Comparison::LE_CMP; }
#line 3653 "parser.cc"
    break;

  case 289:
#line 1047 "parser.yy"
                 { (yyval.comp) = Comparison::GE_CMP; }
#line 3659 "parser.cc"
    break;

  case 290:
#line 1048 "parser.yy"
                  {(yyval.comp) = Comparison::GT_CMP; }
#line 3665 "parser.cc"
    break;

  case 291:
#line 1055 "parser.yy"
               { (yyval.expr) = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3671 "parser.cc"
    break;

  case 292:
#line 1056 "parser.yy"
                                { (yyval.expr) = new Addition(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3677 "parser.cc"
    break;

  case 293:
#line 1057 "parser.yy"
                                    { (yyval.expr) = make_subtraction(*(yyvsp[-2].expr), (yyvsp[-1].expr)); }
#line 3683 "parser.cc"
    break;

  case 294:
#line 1058 "parser.yy"
                                { (yyval.expr) = new Multiplication(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3689 "parser.cc"
    break;

  case 295:
#line 1059 "parser.yy"
                                { (yyval.expr) = new Division(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3695 "parser.cc"
    break;

  case 296:
#line 1060 "parser.yy"
               { (yyval.expr) = (yyvsp[0].appl); }
#line 3701 "parser.cc"
    break;

  case 297:
#line 1064 "parser.yy"
                  { require_fluents(); eq_expr = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3707 "parser.cc"
    break;

  case 298:
#line 1065 "parser.yy"
                        { require_fluents(); }
#line 3713 "parser.cc"
    break;

  case 299:
#line 1066 "parser.yy"
                  { eq_expr = new Addition(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3719 "parser.cc"
    break;

  case 300:
#line 1067 "parser.yy"
                        { require_fluents(); }
#line 3725 "parser.cc"
    break;

  case 301:
#line 1068 "parser.yy"
                  { eq_expr = make_subtraction(*(yyvsp[-2].expr), (yyvsp[-1].expr)); }
#line 3731 "parser.cc"
    break;

  case 302:
#line 1069 "parser.yy"
                        { require_fluents(); }
#line 3737 "parser.cc"
    break;

  case 303:
#line 1070 "parser.yy"
                  { eq_expr = new Multiplication(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3743 "parser.cc"
    break;

  case 304:
#line 1071 "parser.yy"
                        { require_fluents(); }
#line 3749 "parser.cc"
    break;

  case 305:
#line 1072 "parser.yy"
                  { eq_expr = new Division(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3755 "parser.cc"
    break;

  case 306:
#line 1073 "parser.yy"
                             { require_fluents(); prepare_application((yyvsp[0].str)); }
#line 3761 "parser.cc"
    break;

  case 307:
#line 1074 "parser.yy"
                            { eq_expr = make_application(); }
#line 3767 "parser.cc"
    break;

  case 308:
#line 1075 "parser.yy"
                     { make_eq_name((yyvsp[0].str)); }
#line 3773 "parser.cc"
    break;

  case 309:
#line 1076 "parser.yy"
                         { eq_term = make_term((yyvsp[0].str)); eq_expr = NULL; }
#line 3779 "parser.cc"
    break;

  case 310:
#line 1079 "parser.yy"
                        { (yyval.expr) = NULL; }
#line 3785 "parser.cc"
    break;

  case 312:
#line 1083 "parser.yy"
                      { prepare_application((yyvsp[0].str)); }
#line 3791 "parser.cc"
    break;

  case 313:
#line 1084 "parser.yy"
           { (yyval.appl) = make_application(); }
#line 3797 "parser.cc"
    break;

  case 314:
#line 1085 "parser.yy"
                  { prepare_application((yyvsp[0].str)); (yyval.appl) = make_application(); }
#line 3803 "parser.cc"
    break;

  case 315:
#line 1088 "parser.yy"
                      { (yyval.expr) = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3809 "parser.cc"
    break;

  case 316:
#line 1090 "parser.yy"
                 { (yyval.expr) = new Addition(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3815 "parser.cc"
    break;

  case 317:
#line 1092 "parser.yy"
                 { (yyval.expr) = make_subtraction(*(yyvsp[-2].expr), (yyvsp[-1].expr)); }
#line 3821 "parser.cc"
    break;

  case 318:
#line 1094 "parser.yy"
                 { (yyval.expr) = new Multiplication(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3827 "parser.cc"
    break;

  case 319:
#line 1096 "parser.yy"
                 { (yyval.expr) = new Division(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3833 "parser.cc"
    break;

  case 320:
#line 1097 "parser.yy"
                             { (yyval.expr) = (yyvsp[0].appl); }
#line 3839 "parser.cc"
    break;

  case 321:
#line 1099 "parser.yy"
                 { prepare_application((yyvsp[0].str)); (yyval.expr) = make_application(); }
#line 3845 "parser.cc"
    break;

  case 322:
#line 1102 "parser.yy"
                               { (yyval.expr) = NULL; }
#line 3851 "parser.cc"
    break;

  case 324:
#line 1106 "parser.yy"
                             { prepare_application((yyvsp[0].str)); }
#line 3857 "parser.cc"
    break;

  case 325:
#line 1107 "parser.yy"
                  { (yyval.appl) = make_application(); }
#line 3863 "parser.cc"
    break;

  case 326:
#line 1109 "parser.yy"
                    { prepare_application((yyvsp[0].str)); (yyval.appl) = make_application(); }
#line 3869 "parser.cc"
    break;

  case 328:
#line 1117 "parser.yy"
                   { add_term((yyvsp[0].str)); }
#line 3875 "parser.cc"
    break;

  case 329:
#line 1118 "parser.yy"
                       { add_term((yyvsp[0].str)); }
#line 3881 "parser.cc"
    break;

  case 331:
#line 1122 "parser.yy"
                   { add_term((yyvsp[0].str)); }
#line 3887 "parser.cc"
    break;

  case 333:
#line 1126 "parser.yy"
                         { add_variables((yyvsp[0].strs), OBJECT_TYPE); }
#line 3893 "parser.cc"
    break;

  case 334:
#line 1127 "parser.yy"
                                   { add_variables((yyvsp[-1].strs), (yyvsp[0].type)); }
#line 3899 "parser.cc"
    break;

  case 336:
#line 1130 "parser.yy"
                        { (yyval.strs) = new std::vector<const std::string*>(1, (yyvsp[0].str)); }
#line 3905 "parser.cc"
    break;

  case 337:
#line 1131 "parser.yy"
                                     { (yyval.strs) = (yyvsp[-1].strs); (yyval.strs)->push_back((yyvsp[0].str)); }
#line 3911 "parser.cc"
    break;

  case 339:
#line 1135 "parser.yy"
                       { add_names((yyvsp[0].strs), OBJECT_TYPE); }
#line 3917 "parser.cc"
    break;

  case 340:
#line 1136 "parser.yy"
                                 { add_names((yyvsp[-1].strs), (yyvsp[0].type)); }
#line 3923 "parser.cc"
    break;

  case 342:
#line 1139 "parser.yy"
                { (yyval.strs) = new std::vector<const std::string*>(1, (yyvsp[0].str)); }
#line 3929 "parser.cc"
    break;

  case 343:
#line 1140 "parser.yy"
                         { (yyval.strs) = (yyvsp[-1].strs); (yyval.strs)->push_back((yyvsp[0].str)); }
#line 3935 "parser.cc"
    break;

  case 344:
#line 1143 "parser.yy"
                { require_typing(); }
#line 3941 "parser.cc"
    break;

  case 345:
#line 1143 "parser.yy"
                                           { (yyval.type) = (yyvsp[0].type); }
#line 3947 "parser.cc"
    break;

  case 346:
#line 1146 "parser.yy"
              { (yyval.type) = OBJECT_TYPE; }
#line 3953 "parser.cc"
    break;

  case 347:
#line 1147 "parser.yy"
                 { (yyval.type) = make_type((yyvsp[0].str)); }
#line 3959 "parser.cc"
    break;

  case 348:
#line 1148 "parser.yy"
                            { (yyval.type) = make_type(*(yyvsp[-1].types)); delete (yyvsp[-1].types); }
#line 3965 "parser.cc"
    break;

  case 349:
#line 1151 "parser.yy"
               { (yyval.types) = new TypeSet(); }
#line 3971 "parser.cc"
    break;

  case 350:
#line 1152 "parser.yy"
                  { (yyval.types) = new TypeSet(); (yyval.types)->insert(make_type((yyvsp[0].str))); }
#line 3977 "parser.cc"
    break;

  case 351:
#line 1153 "parser.yy"
                     { (yyval.types) = (yyvsp[-1].types); }
#line 3983 "parser.cc"
    break;

  case 352:
#line 1154 "parser.yy"
                        { (yyval.types) = (yyvsp[-1].types); (yyval.types)->insert(make_type((yyvsp[0].str))); }
#line 3989 "parser.cc"
    break;

  case 354:
#line 1164 "parser.yy"
                { delete (yyvsp[0].str); }
#line 3995 "parser.cc"
    break;

  case 355:
#line 1167 "parser.yy"
                      { delete (yyvsp[0].str); }
#line 4001 "parser.cc"
    break;

  case 356:
#line 1170 "parser.yy"
                  { delete (yyvsp[0].str); }
#line 4007 "parser.cc"
    break;

  case 357:
#line 1173 "parser.yy"
            { delete (yyvsp[0].str); }
#line 4013 "parser.cc"
    break;

  case 358:
#line 1176 "parser.yy"
          { delete (yyvsp[0].str); }
#line 4019 "parser.cc"
    break;

  case 359:
#line 1179 "parser.yy"
          { delete (yyvsp[0].str); }
#line 4025 "parser.cc"
    break;

  case 360:
#line 1182 "parser.yy"
        { delete (yyvsp[0].str); }
#line 4031 "parser.cc"
    break;

  case 361:
#line 1185 "parser.yy"
              { delete (yyvsp[0].str); }
#line 4037 "parser.cc"
    break;

  case 362:
#line 1188 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4043 "parser.cc"
    break;

  case 363:
#line 1191 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4049 "parser.cc"
    break;

  case 364:
#line 1194 "parser.yy"
                              { delete (yyvsp[0].str); }
#line 4055 "parser.cc"
    break;

  case 365:
#line 1197 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4061 "parser.cc"
    break;

  case 366:
#line 1200 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4067 "parser.cc"
    break;

  case 367:
#line 1203 "parser.yy"
                        { delete (yyvsp[0].str); }
#line 4073 "parser.cc"
    break;

  case 368:
#line 1206 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4079 "parser.cc"
    break;

  case 369:
#line 1209 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4085 "parser.cc"
    break;

  case 370:
#line 1212 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4091 "parser.cc"
    break;

  case 371:
#line 1215 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4097 "parser.cc"
    break;

  case 372:
#line 1218 "parser.yy"
                      { delete (yyvsp[0].str); }
#line 4103 "parser.cc"
    break;

  case 373:
#line 1221 "parser.yy"
                      { delete (yyvsp[0].str); }
#line 4109 "parser.cc"
    break;

  case 374:
#line 1224 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4115 "parser.cc"
    break;

  case 375:
#line 1227 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4121 "parser.cc"
    break;


#line 4125 "parser.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1256 "parser.yy"


/* Outputs an error message. */
static void yyerror(const std::string& s) {
  std::cerr << PACKAGE ":" << current_file << ':' << line_number << ": " << s
	    << std::endl;
  success = false;
}


/* Outputs a warning. */
static void yywarning(const std::string& s) {
  if (warning_level > 0) {
    std::cerr << PACKAGE ":" << current_file << ':' << line_number << ": " << s
	      << std::endl;
    if (warning_level > 1) {
      success = false;
    }
  }
}


/* Creates an empty domain with the given name. */
static void make_domain(const std::string* name) {
  domain = new Domain(*name);
  domains[*name] = domain;
  requirements = &domain->requirements;
  problem = NULL;
  delete name;
}



/* Creates an empty problem with the given name. */
static void make_problem(const std::string* name,
			 const std::string* domain_name) {
  std::map<std::string, Domain*>::const_iterator di =
    domains.find(*domain_name);
  if (di != domains.end()) {
    domain = (*di).second;
  } else {
    domain = new Domain(*domain_name);
    domains[*domain_name] = domain;
    yyerror("undeclared domain `" + *domain_name + "' used");
  }
  requirements = new Requirements(domain->requirements);
  problem = new Problem(*name, *domain);
  my_problem = problem;
   if (requirements->rewards) {
     Action* noopAct = new Action("noop_action");
     const Application& reward_appl =
       Application::make_application(reward_function, TermList());
     const Assignment* reward_assignment =
       new Assignment(Assignment::ASSIGN_OP, reward_appl, *new Value(0.0));
     noopAct->set_effect(*new AssignmentEffect(*reward_assignment));
     //add_action(noopAct);
     problem->add_action(noopAct);
     
//     const Application& reward_appl =
//       Application::make_application(reward_function, TermList());
//     const Assignment* reward_assignment =
//       new Assignment(Assignment::ASSIGN_OP, reward_appl, *new Value(0.0));
//     problem->add_init_effect(*new AssignmentEffect(*reward_assignment));
   }
  delete name;
  delete domain_name;
}


/* Adds :typing to the requirements. */
static void require_typing() {
  if (!requirements->typing) {
    yywarning("assuming `:typing' requirement");
    requirements->typing = true;
  }
}


/* Adds :fluents to the requirements. */
static void require_fluents() {
  if (!requirements->fluents) {
    yywarning("assuming `:fluents' requirement");
    requirements->fluents = true;
  }
}


/* Adds :disjunctive-preconditions to the requirements. */
static void require_disjunction() {
  if (!requirements->disjunctive_preconditions) {
    yywarning("assuming `:disjunctive-preconditions' requirement");
    requirements->disjunctive_preconditions = true;
  }
}


/* Adds :conditional-effects to the requirements. */ 
static void require_conditional_effects() {
  if (!requirements->conditional_effects) {
    yywarning("assuming `:conditional-effects' requirement");
    requirements->conditional_effects = true;
  }
}


/* Returns a simple type with the given name. */
static Type make_type(const std::string* name) {
  std::pair<Type, bool> t = domain->types().find_type(*name);
  if (!t.second) {
    t.first = domain->types().add_type(*name);
    if (name_kind != TYPE_KIND) {
      yywarning("implicit declaration of type `" + *name + "'");
    }
  }
  delete name;
  return t.first;
}


/* Returns the union of the given types. */
static Type make_type(const TypeSet& types) {
  return domain->types().add_type(types);
}


/* Returns a simple term with the given name. */
static Term make_term(const std::string* name) {
  if ((*name)[0] == '?') {
    std::pair<Variable, bool> v = context.find(*name);
    if (!v.second) {
      if (problem != NULL) {
	v.first = problem->terms().add_variable(OBJECT_TYPE);
      } else {
	v.first = domain->terms().add_variable(OBJECT_TYPE);
      }
      context.insert(*name, v.first);
      yyerror("free variable `" + *name + "' used");
    }
    delete name;
    return v.first;
  } else {
    TermTable& terms = (problem != NULL) ? problem->terms() : domain->terms();
    
    
    const PredicateTable& predicates = (parsing_obs_token ? 
					domain->observables() :
					domain->predicates());
    std::pair<Object, bool> o = terms.find_object(*name);
    if (!o.second) {
      size_t n = term_parameters.size();
      if (parsing_atom && predicates.arity(atom_predicate) > n) {
	o.first = terms.add_object(*name,
				   predicates.parameter(atom_predicate, n));
      } else {
	o.first = terms.add_object(*name, OBJECT_TYPE);
      }
      yywarning("implicit declaration of object `" + *name + "'");
    }
    delete name;
    return o.first;
  }
}


/* Creates a predicate with the given name. */
static void make_predicate(const std::string* name) {
  repeated_predicate = false;
  std::pair<Predicate, bool> p = domain->predicates().find_predicate(*name);
  if (!p.second) {
    //    cout << "Make pred " << *name << endl;
    p.first = domain->predicates().add_predicate(*name);
  } else {
    repeated_predicate = true;
    yywarning("ignoring repeated declaration of predicate `" + *name + "'");
  }
  predicate = p.first;
  parsing_predicate = true;
  delete name;
}

/* Creates a observation token with the given name. */
static void make_observable(const std::string* name) {
  repeated_predicate = false;
  std::cout << "observable: " << *name <<std::endl;
  std::pair<Predicate, bool> p = domain->observables().find_predicate(*name);
  if (!p.second) {
    p.first = domain->observables().add_predicate(*name);
  } else {
    repeated_predicate = true;
    yywarning("ignoring repeated declaration of observable `" + *name + "'");
  }
  predicate = p.first;
  //  parsing_obs_token = true;
  parsing_predicate = true;
  delete name;
}

//static ObservationCptRow* make_observation(
static ObservationEntry* make_observation(
					   const StateFormula& form,
					   const ProbabilisticEffect& eff){
  //  ObservationCptRow *row = new ObservationCptRow(form, eff);
  OBS_TYPE=OBS_CPT;
  //  const Rational& prob = eff.probability(0);
  // const Atom& symbol = ((SimpleEffect&)eff.effect(0)).atom();
  ObservationEntry *row = new ObservationEntry(form, eff);
  return row;
}

/* Appends new observation entry to list of observation entries */
static ObservationEntry* make_observation(
			       const StateFormula& form,
			       const Rational& posprob,
			       const Rational& negprob){
  //  cout << "make entry"<<endl;
  OBS_TYPE=OBS_TPFP;
  if (typeid(form) == typeid(Constant)) {
    std::cout << "token is cosnt"<<std::endl;
  }
//   token.print(cout, ((Domain)problem->domain()).observables(),
// 	      problem->domain().functions(),
// 	      problem->terms());
  ObservationEntry *ob = new ObservationEntry(form, posprob, negprob);

  
  return ob;
}

/* Appends new observation entry to list of observation entries */
static ObservationEntry* make_observation(
			       const StateFormula& form,
			       const StateFormula& o,
			       const Rational& prob){
  //  cout << "make entry"<<endl;
  OBS_TYPE=OBS_TPFP;
  if (typeid(form) == typeid(Constant)) {
    std::cout << "token is cosnt"<<std::endl;
  }

  if (typeid(o) == typeid(Constant)) {
    std::cout << "token is cosnt"<<std::endl;
  }

  //  o.print(cout, ((Domain)problem->domain()).predicates(),
  //	      problem->domain().functions(),
  //	      problem->terms());

//   token
  ObservationEntry *ob = new ObservationEntry(form, &o, prob, prob);

  
  return ob;
}

/* Creates a function with the given name. */
static void make_function(const std::string* name) {
  repeated_function = false;
  std::pair<Function, bool> f = domain->functions().find_function(*name);
  if (!f.second) {
    f.first = domain->functions().add_function(*name);
  } else {
    repeated_function = true;
    if (requirements->rewards && f.first == reward_function) {
      yywarning("ignoring declaration of reserved function `reward'");
    } else {
      yywarning("ignoring repeated declaration of function `" + *name + "'");
    }
  }
  function = f.first;
  parsing_function = true;
  delete name;
}


/* Creates an action with the given name. */
static void make_action(const std::string* name) {
  context.push_frame();
  action = new ActionSchema(*name);
  delete name;
}


/* Adds the current action to the current domain. */
static void add_action() {
  context.pop_frame();
  if (domain->find_action(action->name()) == NULL) {
    domain->add_action(*action);
  } else {
    yywarning("ignoring repeated declaration of action `"
	      + action->name() + "'");
    delete action;
  }
  action = NULL;
}

/* Adds the current event to the current domain. */
static void add_event() {
  context.pop_frame();
  if (domain->find_event(action->name()) == NULL) {
    domain->add_event(*action);
  } else {
    yywarning("ignoring repeated declaration of event `"
	      + action->name() + "'");
    delete action;
  }
  action = NULL;
}

/* Prepares for the parsing of a universally quantified effect. */ 
static void prepare_forall_effect() {
  if (!requirements->conditional_effects) {
    yywarning("assuming `:conditional-effects' requirement");
    requirements->conditional_effects = true;
  }
  context.push_frame();
  quantified.push_back(NULL_TERM);
}


/* Creates a universally quantified effect. */
static const pEffect* make_forall_effect(const pEffect& effect) {
  context.pop_frame();
  QuantifiedEffect* qeffect = new QuantifiedEffect(effect);
  size_t n = quantified.size() - 1;
  size_t m = n;
  while (is_variable(quantified[n])) {
    n--;
  }
  for (size_t i = n + 1; i <= m; i++) {
    qeffect->add_parameter(quantified[i]);
  }
  quantified.resize(n);
  return qeffect;
}


/* Adds an outcome to the given probabilistic effect. */
static void add_effect_outcome(ProbabilisticEffect& peffect,
			        const Rational* p, const pEffect& effect) {
   if((*p == -1.0 || *p == -2.0 || *p == -3.0) 
      && !requirements->non_deterministic
     ){
    yywarning("assuming `:non-deterministic' requirement");
    requirements->non_deterministic = true;    
    // requirements->probabilistic_effects = true;
  }
  
   else if ((*p != -1.0 && *p != -2.0 || *p != -3.0) &&
	    !requirements->probabilistic_effects) {
    yywarning("assuming `:probabilistic-effects' requirement");
    requirements->probabilistic_effects = true;
    
  } 


  if(*p == -1.0){ // okay, its an oneof nd-effect
  }
  else if(*p == -2.0){ // okay, its an unknown nd-effect
  }
  else if(*p == -3.0){ // okay, its an or nd-effect
  }
  else if (*p < 0.0 || 
	   *p > 1.0) {
    yyerror("outcome probability needs to be in the interval [0,1]");
  }
  if (!peffect.add_outcome(*p, effect)) {
    yyerror("effect outcome probabilities add up to more than 1");
  }

  delete p;
}
static void add_feffect_outcome(ProbabilisticEffect& peffect,
			        const Expression* p, const pEffect& effect) {
   
  if (!requirements->probabilistic_effects) {
    yywarning("assuming `:probabilistic-effects' requirement");
    requirements->probabilistic_effects = true;
  }
 //  if (*p < 0 || *p > 1) {
//     yyerror("outcome probability needs to be in the interval [0,1]");
//   }
  if (!peffect.add_foutcome(*p, effect)) {
    yyerror("effect outcome probabilities add up to more than 1");
  }
  //cout << "done adding feffect" <<endl;
  // delete p;
}

/* Creates an add effect. */
static const pEffect* make_add_effect(const Atom& atom) {
  domain->predicates().make_dynamic(atom.predicate());
  return new AddEffect(atom);
}


/* Creates a delete effect. */
static const pEffect* make_delete_effect(const Atom& atom) {
  domain->predicates().make_dynamic(atom.predicate());
  return new DeleteEffect(atom);
}


/* Creates an assignment effect. */
static const pEffect* make_assignment_effect(Assignment::AssignOp oper,
					    const Application& application,
					    const Expression& expr) {
  if (requirements->rewards && application.function() == reward_function) {
    if ((oper != Assignment::INCREASE_OP && oper != Assignment::DECREASE_OP)
	|| typeid(expr) != typeid(Value)) {
      yyerror("only constant reward increments/decrements allowed");
    }
  } else {
    require_fluents();
  }
  effect_fluent = false;
  domain->functions().make_dynamic(application.function());
  const Assignment& assignment = *new Assignment(oper, application, expr);
  return new AssignmentEffect(assignment);
}


/* Adds types, constants, or objects to the current domain or problem. */
static void add_names(const std::vector<const std::string*>* names,
		      Type type) {
  for (std::vector<const std::string*>::const_iterator si = names->begin();
       si != names->end(); si++) {
    const std::string* s = *si;
    if (name_kind == TYPE_KIND) {
      if (*s == OBJECT_NAME) {
	yywarning("ignoring declaration of reserved type `object'");
      } else if (*s == NUMBER_NAME) {
	yywarning("ignoring declaration of reserved type `number'");
      } else {
	std::pair<Type, bool> t = domain->types().find_type(*s);
	if (!t.second) {
	  t.first = domain->types().add_type(*s);
	}
	if (!domain->types().add_supertype(t.first, type)) {
	  yyerror("cyclic type hierarchy");
	}
      }
    } else if (name_kind == CONSTANT_KIND) {
      std::pair<Object, bool> o = domain->terms().find_object(*s);
      if (!o.second) {
	domain->terms().add_object(*s, type);
      } else {
	TypeSet components;
	domain->types().components(components, domain->terms().type(o.first));
	components.insert(type);
	domain->terms().set_type(o.first, make_type(components));
      }
    } else { /* name_kind == OBJECT_KIND */
      if (domain->terms().find_object(*s).second) {
	yywarning("ignoring declaration of object `" + *s
		  + "' previously declared as constant");
      } else {
	std::pair<Object, bool> o = problem->terms().find_object(*s);
	if (!o.second) {
	  problem->terms().add_object(*s, type);
	} else {
	  TypeSet components;
	  domain->types().components(components,
				     problem->terms().type(o.first));
	  components.insert(type);
	  problem->terms().set_type(o.first, make_type(components));
	}
      }
    }
    delete s;
  }
  delete names;
}


/* Adds variables to the current variable list. */
static void add_variables(const std::vector<const std::string*>* names,
			  Type type) {
  for (std::vector<const std::string*>::const_iterator si = names->begin();
       si != names->end(); si++) {
    const std::string* s = *si;
    if (parsing_predicate && !parsing_obs_token) {
      if (!repeated_predicate) {
	domain->predicates().add_parameter(predicate, type);
      }
    } else if (parsing_function && !parsing_obs_token ) {
      if (!repeated_function) {
	domain->functions().add_parameter(function, type);
      }
    }
    else if(parsing_predicate && parsing_obs_token){
      if (!repeated_function) {
	domain->observables().add_parameter(function, type);
      }
    }
    else {
      if (context.shallow_find(*s).second) {
	yyerror("repetition of parameter `" + *s + "'");
      } else if (context.find(*s).second) {
	yywarning("shadowing parameter `" + *s + "'");
      }
      Variable var;
      if (problem != NULL) {
	var = problem->terms().add_variable(type);
      } else {
	var = domain->terms().add_variable(type);
      }
      context.insert(*s, var);
      if (!quantified.empty()) {
	quantified.push_back(var);
      } else { /* action != NULL */
	action->add_parameter(var);
      }
    }
    delete s;
  }
  delete names;
}


/* Prepares for the parsing of an atomic state formula. */ 
static void prepare_atom(const std::string* name) {
  std::pair<Predicate, bool> p;
  if(parsing_obs_token)
     p = domain->observables().find_predicate(*name);
  else
     p = domain->predicates().find_predicate(*name);
  if (!p.second) {
    if(parsing_obs_token){
      atom_predicate = domain->observables().add_predicate(*name);      
    }
    else
      atom_predicate = domain->predicates().add_predicate(*name);
    undeclared_atom_predicate = true;
    if (problem != NULL) {
      yywarning("undeclared predicate `" + *name + "' used");
    } else {
      yywarning("implicit declaration of predicate `" + *name + "'");
    }
  } else {
    atom_predicate = p.first;
    undeclared_atom_predicate = false;
  }
  term_parameters.clear();
  parsing_atom = true;
  delete name;
}


/* Prepares for the parsing of a function application. */ 
static void prepare_application(const std::string* name) {
  std::pair<Function, bool> f = domain->functions().find_function(*name);
  if (!f.second) {
    appl_function = domain->functions().add_function(*name);
    undeclared_appl_function = true;
    if (problem != NULL) {
      yywarning("undeclared function `" + *name + "' used");
    } else {
      yywarning("implicit declaration of function `" + *name + "'");
    }
  } else {
    appl_function = f.first;
    undeclared_appl_function = false;
  }
  if (requirements->rewards && f.first == reward_function) {
    if (!effect_fluent && !metric_fluent) {
      yyerror("reserved function `reward' not allowed here");
    }
  } else {
    require_fluents();
  }
  term_parameters.clear();
  parsing_application = true;
  delete name;
}


/* Adds a term with the given name to the current atomic state formula. */
static void add_term(const std::string* name) {
  Term term = make_term(name);
  const TermTable& terms =
    (problem != NULL) ? problem->terms() : domain->terms();
  if (parsing_atom) {
    PredicateTable& predicates = (!parsing_obs_token ? 
				  domain->predicates() :
				  domain->observables());
    size_t n = term_parameters.size();
    if (undeclared_atom_predicate) {
      predicates.add_parameter(atom_predicate, terms.type(term));
    } else if (predicates.arity(atom_predicate) > n
	       && !domain->types().subtype(terms.type(term),
					   predicates.parameter(atom_predicate,
								n))) {
      yyerror("type mismatch");
    }
  } else if (parsing_application) {
    FunctionTable& functions = domain->functions();
    size_t n = term_parameters.size();
    if (undeclared_appl_function) {
      functions.add_parameter(appl_function, terms.type(term));
    } else if (functions.arity(appl_function) > n
	       && !domain->types().subtype(terms.type(term),
					   functions.parameter(appl_function,
							       n))) {
      yyerror("type mismatch");
    }
  }
  term_parameters.push_back(term);
}


/* Creates the atomic formula just parsed. */
static const Atom* make_atom() {
  size_t n = term_parameters.size();
  if(parsing_obs_token){

    if (domain->observables().arity(atom_predicate) < n) {
      yyerror("too many parameters passed to obs `"
	      + domain->observables().name(atom_predicate) + "'");
    } else if (domain->observables().arity(atom_predicate) > n) {
      yyerror("too few parameters passed to obs `"
	      + domain->observables().name(atom_predicate) + "'");
    }
  }
  else{
    if (domain->predicates().arity(atom_predicate) < n) {
      yyerror("too many parameters passed to predicate `"
	      + domain->predicates().name(atom_predicate) + "'");
    } else if (domain->predicates().arity(atom_predicate) > n) {
      yyerror("too few parameters passed to predicate `"
	      + domain->predicates().name(atom_predicate) + "'");
    }
  }
  parsing_atom = false;
  return &Atom::make_atom(atom_predicate, term_parameters);
}


/* Creates the function application just parsed. */
static const Application* make_application() {
  size_t n = term_parameters.size();
  if (domain->functions().arity(appl_function) < n) {
    yyerror("too many parameters passed to function `"
	    + domain->functions().name(appl_function) + "'");
  } else if (domain->functions().arity(appl_function) > n) {
    yyerror("too few parameters passed to function `"
	    + domain->functions().name(appl_function) + "'");
  }
  parsing_application = false;
  return &Application::make_application(appl_function, term_parameters);
}


/* Creates a subtraction. */
static const Expression* make_subtraction(const Expression& term,
					  const Expression* opt_term) {
  if (opt_term != NULL) {
    return new Subtraction(term, *opt_term);
  } else {
    return new Subtraction(*new Value(0.0), term);
  }
}


/* Creates an atom or fluent for the given name to be used in an
   equality formula. */
static void make_eq_name(const std::string* name) {
  std::pair<Function, bool> f = domain->functions().find_function(*name);
  if (f.second) {
    prepare_application(name);
    eq_expr = make_application();
  } else {
    /* Assume this is a term. */
    eq_term = make_term(name);
    eq_expr = NULL;
  }
}


/* Creates an equality formula. */
static const StateFormula* make_equality() {
  if (!requirements->equality) {
    yywarning("assuming `:equality' requirement");
    requirements->equality = true;
  }
  if (first_eq_expr != NULL && eq_expr != NULL) {
    return new Comparison(Comparison::EQ_CMP, *first_eq_expr, *eq_expr);
  } else if (first_eq_expr == NULL && eq_expr == NULL) {
    const TermTable& terms =
      (problem != NULL) ? problem->terms() : domain->terms();
    if (domain->types().subtype(terms.type(first_eq_term), terms.type(eq_term))
	|| domain->types().subtype(terms.type(eq_term),
				   terms.type(first_eq_term))) {
      return new Equality(first_eq_term, eq_term);
    } else {
      return 0;//&StateFormula::FALSE;
    }
  } else {
    yyerror("comparison of term and numeric expression");
    return 0;//&StateFormula::FALSE;
  }
}


/* Creates a negated formula. */
static const StateFormula* make_negation(const StateFormula& negand) {
  if (typeid(negand) == typeid(Atom)) {
    if (!requirements->negative_preconditions) {
      yywarning("assuming `:negative-preconditions' requirement");
      requirements->negative_preconditions = true;
    }
  } else if (typeid(negand) != typeid(Equality)
	     && typeid(negand) != typeid(Comparison)) {
    require_disjunction();
  }
  return &Negation::make_negation(negand);
}


/* Creates an implication. */
static const StateFormula* make_implication(const StateFormula& f1,
					    const StateFormula& f2) {
  require_disjunction();
  Disjunction* disj = new Disjunction();
  disj->add_disjunct(Negation::make_negation(f1));
  disj->add_disjunct(f2);
  return disj;
}


/* Prepares for the parsing of an existentially quantified formula. */
static void prepare_exists() {
  if (!requirements->existential_preconditions) {
    yywarning("assuming `:existential-preconditions' requirement");
    requirements->existential_preconditions = true;
  }
  context.push_frame();
  quantified.push_back(NULL_TERM);
}


/* Prepares for the parsing of a universally quantified formula. */
static void prepare_forall() {
  if (!requirements->universal_preconditions) {
    yywarning("assuming `:universal-preconditions' requirement");
    requirements->universal_preconditions = true;
  }
  context.push_frame();
  quantified.push_back(NULL_TERM);
}


/* Creates an existentially quantified formula. */
static const StateFormula* make_exists(const StateFormula& body) {
  context.pop_frame();
  size_t m = quantified.size() - 1;
  size_t n = m;
  while (is_variable(quantified[n])) {
    n--;
  }
  if (n < m) {
    Exists* exists = new Exists();
    for (size_t i = n + 1; i <= m; i++) {
      exists->add_parameter(quantified[i]);
    }
    exists->set_body(body);
    quantified.resize(n);
    return exists;
  } else {
    quantified.pop_back();
    return &body;
  }
}


/* Creates a universally quantified formula. */
static const StateFormula* make_forall(const StateFormula& body) {
  context.pop_frame();
  size_t m = quantified.size() - 1;
  size_t n = m;
  while (is_variable(quantified[n])) {
    n--;
  }
  if (n < m) {
    Forall* forall = new Forall();
    for (size_t i = n + 1; i <= m; i++) {
      forall->add_parameter(quantified[i]);
    }
    forall->set_body(body);
    quantified.resize(n);
    return forall;
  } else {
    quantified.pop_back();
    return &body;
  }
}

void set_discount(const Rational& discount){
  problem->set_discount(discount);
}


/* Sets the goal reward for the current problem. */
void set_goal_reward(const Expression& goal_reward) {
  if (!requirements->rewards) {
    yyerror("goal reward only allowed with the `:rewards' requirement");
  } else {
    const Application& reward_appl =
      Application::make_application(reward_function, TermList());
    const Assignment* reward_assignment =
      new Assignment(Assignment::INCREASE_OP, reward_appl, goal_reward);
    problem->set_goal_reward(*reward_assignment);
  }
}


/* Sets the default metric for the current problem. */
static void set_default_metric() {
  if (requirements->rewards) {
    const Application& reward_appl =
      Application::make_application(reward_function, TermList());
    problem->set_metric(reward_appl);
  }
}

/* make all atoms in formula or subformula dynamic */
void make_all_dynamic(const StateFormula &formula){
   if (typeid(formula) == typeid(Constant)) {
    /*
     * The formula is either TRUE or FALSE, so it contains no state
     * variables.
     */
    return;
  }

  const Atom* af = dynamic_cast<const Atom*>(&formula);
  if (af != NULL) {
    /*
     * The formula is an atom representing a single state variable.
     */
    domain->predicates().make_dynamic(af->predicate());
    return;
  }

  const Negation* nf = dynamic_cast<const Negation*>(&formula);
  if (nf != NULL) {
    /*
     * The state variables of a negation are the state variables of the
     * negand.
     */
    make_all_dynamic(nf->negand());
    return;
  }

  const Conjunction* cf = dynamic_cast<const Conjunction*>(&formula);
  if (cf != NULL) {
    /*
     * The state variables of a conjunction are the state variables of
     * the conjuncts.
     */
    for (size_t i = 0; i < cf->size(); i++) {
      make_all_dynamic(cf->conjunct(i));
    }
    return;
  }

  const Disjunction* df = dynamic_cast<const Disjunction*>(&formula);
  if (df != NULL) {
    /*
     * The state variables of a disjunction are the state variables of
     * the disjuncts.
     */
    for (size_t i = 0; i < df->size(); i++) {
      make_all_dynamic(df->disjunct(i));
    }
    return;
  }

  const OneOfDisjunction* odf = dynamic_cast<const OneOfDisjunction*>(&formula);
  if (odf != NULL) {
    /*
     * The state variables of a disjunction are the state variables of
     * the disjuncts.
     */
    for (size_t i = 0; i < odf->size(); i++) {
      make_all_dynamic(odf->disjunct(i));
    }
    return;
  }
  /*
   * No other types of formulas should appear in fully instantiated
   * action preconditions and effect conditions.
   */

}


/* if we use logical formula for init, then need to get atoms holding for later grounding 
   purposes*/
static void get_init_elts(){
  
  const StateFormula &init = problem->init_formula();
  if(!&init)
    return;

  const Conjunction *c = dynamic_cast<const Conjunction*>(&init);
  if(c != NULL){
    for (size_t i = 0; i < c->size(); i++) {
      const Atom *a = dynamic_cast<const Atom*>(&c->conjunct(i));
      if(a != NULL){
	problem->add_init_atom(*a);	
      }
      else{
	make_all_dynamic(c->conjunct(i));
      }      
    }
  }
  else
    make_all_dynamic(init);

  

}
