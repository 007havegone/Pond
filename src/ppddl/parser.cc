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
        for (std::vector<VariableMap>::const_reverse_iterator fi = frames_.rbegin(); fi != frames_.rend(); fi++) {
            VariableMap::const_iterator vi = (*fi).find(name);
            if (vi != (*fi).end()) {
                return std::make_pair((*vi).second, true);
            }
        }
        return std::make_pair(0, false);
    }

private:
    struct VariableMap : public std::map<std::string, Variable> {};
    
    std::vector<VariableMap> frames_;// 存储每一个变量名对应的index即variable
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
/* Paramerers for atomic state formula or function application being parsed. 
  状态变量表，一个domain/problem中使用的状态变量
*/
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

#line 570 "parser.cc"

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
#define YYLAST   1779

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  189
/* YYNRULES -- Number of rules.  */
#define YYNRULES  414
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  701

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
       0,   355,   355,   355,   357,   357,   358,   358,   368,   376,
     386,   392,   395,   402,   409,   410,   411,   412,   413,   418,
     424,   427,   446,   447,   450,   457,   464,   469,   476,   483,
     489,   493,   500,   508,   509,   510,   517,   517,   521,   522,
     523,   524,   527,   528,   529,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   546,   547,   548,   549,   550,   551,
     552,   555,   556,   557,   558,   559,   562,   563,   564,   565,
     566,   567,   570,   571,   572,   575,   576,   577,   578,   579,
     582,   583,   584,   590,   590,   595,   596,   599,   603,   606,
     607,   610,   611,   612,   614,   616,   617,   619,   621,   623,
     624,   625,   630,   631,   632,   634,   636,   638,   645,   650,
     666,   666,   670,   670,   674,   677,   677,   684,   685,   688,
     688,   692,   693,   694,   697,   698,   701,   701,   704,   704,
     708,   709,   713,   712,   725,   725,   728,   728,   735,   736,
     739,   740,   743,   744,   745,   748,   749,   753,   756,   762,
     763,   769,   770,   771,   771,   773,   773,   775,   776,   777,
     778,   781,   782,   785,   790,   792,   798,   805,   807,   811,
     813,   817,   824,   827,   828,   829,   829,   833,   834,   835,
     836,   837,   843,   844,   847,   848,   849,   866,   865,   869,
     868,   875,   876,   882,   885,   886,   887,   887,   888,   892,
     893,   893,   894,   897,   898,   901,   902,   906,   907,   908,
     911,   912,   915,   916,   920,   921,   922,   922,   925,   926,
     929,   930,   934,   934,   938,   939,   943,   943,   945,   944,
     950,   951,   954,   958,   960,   962,   964,   965,   975,   980,
     984,   986,   990,   995,   996,   999,  1000,  1003,  1004,  1008,
    1011,  1012,  1013,  1014,  1017,  1018,  1021,  1022,  1026,  1027,
    1027,  1029,  1029,  1038,  1039,  1041,  1040,  1043,  1043,  1045,
    1046,  1047,  1047,  1048,  1048,  1049,  1050,  1050,  1052,  1052,
    1056,  1057,  1060,  1061,  1063,  1064,  1067,  1067,  1069,  1072,
    1072,  1074,  1077,  1078,  1079,  1080,  1087,  1088,  1089,  1090,
    1091,  1092,  1095,  1097,  1097,  1099,  1099,  1101,  1101,  1103,
    1103,  1105,  1105,  1107,  1108,  1111,  1112,  1115,  1115,  1117,
    1120,  1121,  1123,  1125,  1127,  1129,  1130,  1134,  1135,  1138,
    1138,  1140,  1148,  1149,  1150,  1153,  1154,  1157,  1158,  1159,
    1159,  1162,  1163,  1166,  1167,  1168,  1168,  1171,  1172,  1175,
    1175,  1178,  1179,  1180,  1183,  1184,  1185,  1186,  1189,  1196,
    1199,  1202,  1205,  1208,  1211,  1214,  1217,  1220,  1223,  1226,
    1229,  1232,  1235,  1238,  1241,  1244,  1247,  1250,  1253,  1256,
    1259,  1262,  1262,  1262,  1263,  1264,  1264,  1265,  1268,  1269,
    1269,  1274,  1277,  1277,  1277,  1278,  1278,  1278,  1279,  1279,
    1279,  1279,  1279,  1279,  1279,  1279,  1280,  1280,  1280,  1280,
    1280,  1281,  1281,  1282,  1285
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
  "problem_body", "problem_body_r", "problem_body_ig", "$@20",
  "problem_body_h", "$@21", "problem_body_ho", "problem_body_hd",
  "problem_body_o", "problem_body_oh", "problem_body_od", "problem_body_d",
  "$@22", "problem_body_dh", "problem_body_do", "object_decl", "$@23",
  "horizon_decl", "init", "$@24", "$@25", "init_elements", "init_element",
  "prob_inits", "oneof_inits", "unknown_inits", "simple_init", "one_inits",
  "one_init", "value", "goal_spec", "discount", "goal_reward",
  "metric_spec", "$@26", "$@27", "formula", "$@28", "$@29", "$@30", "$@31",
  "$@32", "$@33", "conjuncts", "disjuncts", "oneof_disjuncts",
  "atomic_term_formula", "$@34", "atomic_name_formula", "$@35",
  "binary_comp", "f_exp", "term_or_f_exp", "$@36", "$@37", "$@38", "$@39",
  "$@40", "opt_f_exp", "f_head", "$@41", "ground_f_exp",
  "opt_ground_f_exp", "ground_f_head", "$@42", "terms", "names",
  "variables", "$@43", "variable_seq", "typed_names", "$@44", "name_seq",
  "type_spec", "$@45", "type", "types", "function_type", "define",
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

#define YYPACT_NINF (-520)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-12)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -520,    32,  -520,     5,    64,  -520,    67,   151,  -520,   657,
    -520,   168,  -520,  -520,   117,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,   685,  1416,  -520,  1426,  -520,
    -520,  -520,   123,   141,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  1203,  -520,  -520,  -520,  1426,  -520,
    -520,  1426,  -520,  -520,  -520,  -520,   158,   158,   147,    78,
    1626,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,  -520,   369,  -520,  -520,  -520,
    1278,   154,   325,  -520,  1426,   162,   171,  -520,   167,   175,
     217,  -520,  -520,  1626,  1626,   189,   474,  -520,  -520,  -520,
    -520,  -520,  -520,  1203,  -520,   557,  1278,  -520,  -520,  -520,
    -520,  -520,   510,   196,   216,   216,   915,  -520,  -520,  -520,
    1315,   204,   221,   239,  -520,  -520,  1278,  1278,  1278,  1278,
    -520,   249,  1278,  1278,  1278,  1278,  -520,   260,  -520,  -520,
    -520,   282,    55,  -520,   284,  -520,  -520,  -520,  -520,   839,
    1453,  1453,  -520,   289,   303,  1278,  1278,  1278,  1278,   953,
    -520,  1278,  1278,  1278,  1278,  -520,  -520,  1426,  -520,  -520,
    -520,  1426,   296,  1453,  1453,  1664,  -520,  -520,  -520,  -520,
     307,   360,  1626,  -520,   327,   353,   357,   358,  -520,   359,
     361,   362,   366,   991,   367,   401,   216,   368,  -520,  -520,
    -520,  1453,  1461,  1453,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  1490,  -520,  -520,  1315,  -520,  -520,  -520,  -520,  -520,
     302,   371,  -520,  -520,   370,  -520,   373,   383,   384,   391,
     392,  -520,  1626,   393,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,   418,  -520,  -520,  -520,  -520,
    -520,  -520,   693,  1692,   399,  -520,  -520,   801,  1352,  1426,
     335,   405,  1093,   407,   877,  1747,  -520,  -520,  -520,  -520,
    1626,  1626,  -520,   152,  -520,   477,  -520,   291,  -520,   214,
    -520,   370,   408,   411,   251,  -520,   413,   370,   414,   415,
     122,  -520,   370,   411,   416,   417,   419,  -520,   703,  -520,
    -520,  -520,  -520,  -520,  1626,  1278,  1453,  -520,  -520,  -520,
     216,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  1631,  -520,  1626,  1626,   153,   422,  -520,  -520,   347,
    -520,   414,   370,   370,   232,  -520,   370,   426,  -520,   164,
    -520,    66,  -520,   110,  -520,   370,   370,  -520,   223,  -520,
    -520,  -520,   737,   433,   285,  -520,  -520,   434,   437,   438,
    -520,  -520,   441,   775,  -520,   442,   335,  -520,  -520,  1626,
     444,    56,  -520,   456,   456,  -520,   370,   125,   370,   370,
    -520,   370,   427,   427,  -520,  1562,   445,   737,  -520,    24,
    -520,  -520,  -520,  -520,  -520,  -520,  -520,  1453,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,  -520,   422,  -520,   447,   155,
     155,   370,   160,    23,   448,   449,  -520,   457,   455,  1426,
    -520,  1626,   -24,   458,   459,   460,   463,  -520,  -520,  -520,
    -520,   464,  1626,   216,   216,   470,  -520,   216,  1426,  1453,
     468,   469,  -520,  -520,   170,   461,  -520,   471,   335,  -520,
    -520,  1498,   512,  -520,  -520,   134,   182,  -520,   481,   484,
     485,  -520,   483,  -520,  -520,  -520,  -520,  -520,  1626,  -520,
    -520,   491,   492,  -520,  -520,  -520,   499,  -520,  -520,    -9,
    -520,  -520,  -520,  -520,  -520,   293,  -520,  -520,  -520,  1426,
     209,   237,   -13,  -520,  -520,   233,   236,   496,   511,   506,
    1426,   507,  -520,  -520,  -520,  -520,   267,  -520,    64,   216,
    1525,  1525,  1389,  -520,   158,  -520,  -520,    74,  -520,  -520,
    -520,  -520,   521,   523,  -520,   526,   528,   281,   551,     7,
    -520,  -520,   516,   558,    10,  -520,  -520,   516,  -520,   560,
     737,   552,  -520,   737,  -520,  -520,  -520,   287,   547,   574,
    1101,  -520,  -520,  -520,   549,  -520,  1626,   570,  -520,  1131,
    1525,    82,  -520,  1139,   559,   294,  1241,  1241,   561,   565,
    1626,   566,   567,   313,  -520,   311,   321,   311,   603,  -520,
    -520,   321,  -520,  -520,   568,   571,   737,   572,  -520,  -520,
    1389,  -520,  -520,  -520,  -520,  -520,   573,  -520,  -520,  -520,
    1525,  1029,   409,  -520,  -520,   575,    -1,   575,  -520,  -520,
    -520,  -520,   621,   580,  -520,   581,  -520,  -520,   582,  -520,
    -520,   583,  -520,  -520,   589,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,   602,  -520,   598,  1165,  -520,  -520,   608,
    -520,  -520,  -520,   390,  -520,  1241,  -520,  1241,  1241,  1241,
    1241,  -520,  -520,  -520,  -520,  -520,   609,    49,  -520,  -520,
    1067,  -520,   610,  1241,  1241,  1241,  1241,  -520,  -520,   604,
    -520,   611,   612,   616,   617,   658,  -520,  -520,  -520,  -520,
    -520
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,    33,     0,     0,     1,     3,     0,     5,     0,
       7,     0,    34,    35,     0,   381,   382,   383,   362,   385,
     386,   390,   389,   384,   387,     0,     0,   264,     0,   388,
     288,   359,     0,     0,   273,   363,   364,   365,   366,   367,
     368,   293,   294,   263,     0,   292,   295,   267,     0,   280,
     271,     0,   276,   278,   286,   172,     0,     0,     0,     0,
       0,   284,   392,   393,   394,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     395,   396,   397,   413,   414,   302,     0,   265,   313,   314,
       0,     0,     0,   282,     0,     0,     0,   332,     0,     0,
       0,   360,   361,     0,     0,     0,     0,   305,   303,   307,
     309,   311,   391,     0,   296,     0,     0,   301,   319,   269,
     270,   281,     0,     0,   337,   337,     0,   184,   185,   369,
       0,     0,     0,     0,   274,   285,     0,     0,     0,     0,
     332,     0,     0,     0,     0,     0,   317,     0,   272,   283,
     275,     0,   338,   341,     0,   287,   333,   334,   172,     0,
       0,     0,    36,     0,     0,   315,     0,     0,     0,     0,
     266,   315,     0,     0,     0,   332,   268,     0,   349,   339,
     342,     0,     0,     0,     0,     0,   163,   151,   173,   165,
      38,     0,     0,   316,     0,     0,     0,     0,   312,     0,
       0,     0,     0,     0,     0,     0,   337,     0,   186,   164,
     166,     0,     0,     0,   370,   371,   372,   373,   374,   175,
     155,     0,   161,   153,     0,   177,   178,   179,   180,   181,
       0,     0,    41,    44,    53,    85,    39,    42,    45,    46,
      48,    87,     0,     0,   306,   304,   308,   310,   298,   297,
     299,   300,   318,   277,   378,     0,   350,   351,   352,   340,
     279,   168,     0,     0,     0,   171,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,   112,   117,   115,
       0,     0,    37,     0,    86,     0,    40,     0,    43,     0,
      49,    60,    54,    56,     0,    50,    47,    65,    61,    62,
       0,    52,    71,    66,    67,     0,     0,   379,     0,   158,
     167,   160,   159,   169,     0,     0,     0,   174,   152,   162,
     337,   157,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   102,   103,   104,   105,   106,   107,   108,   109,   101,
     100,     0,    89,   343,   343,     0,   121,   134,   136,     0,
      57,    55,    74,    72,     0,    59,    79,    75,    83,     0,
      51,     0,    63,     0,    64,    82,    80,    69,     0,    70,
      68,   187,     0,     0,     0,   354,   355,     0,     0,     0,
      88,    90,     0,   344,   347,     0,     0,   114,   118,     0,
       0,   122,   124,   138,   138,    58,    73,     0,    76,    77,
     130,    81,     0,     0,    20,     0,     0,     0,    13,     0,
      32,     8,   353,   356,   357,   176,   156,     0,   111,   345,
     348,   113,   119,   128,   116,   126,   121,   125,     0,   145,
     145,    78,     0,     0,     0,     0,   192,   258,     0,     0,
      30,     0,     0,     0,     0,     0,     0,    23,     9,    10,
      12,     0,   343,   337,   337,     0,   123,   337,     0,     0,
       0,     0,   141,   142,   145,   144,   146,     0,     0,    84,
     131,   237,     0,   191,   188,     0,   194,   253,     0,     0,
      28,    31,     0,    18,    14,    15,    16,    17,    21,   154,
     346,     0,     0,   377,   127,   358,     0,   147,   148,     0,
     135,   140,   143,   137,   132,     0,   226,   232,   291,     0,
       0,     0,     0,   193,   196,   199,   214,     0,     0,    26,
       0,     0,    22,   120,   129,   139,     0,   183,     0,   337,
       0,     0,     0,   280,     0,   289,   230,     0,   375,   376,
     261,   259,     0,     0,   222,     0,     0,   207,     0,     0,
     195,   200,   205,     0,     0,   198,   216,   218,   380,     0,
       0,     0,    27,     0,    19,   150,   182,     0,     0,     0,
       0,   241,   243,   247,     0,   242,     0,     0,   331,     0,
       0,     0,   335,     0,     0,   258,     0,     0,     0,     0,
     343,     0,     0,     0,   197,   210,   212,   203,     0,   202,
     206,   220,   215,   219,     0,     0,     0,     0,   149,   133,
       0,   245,   235,   240,   236,   329,     0,   228,   238,   234,
       0,     0,     0,   227,   231,   258,     0,   258,   250,   256,
     326,   320,     0,     0,   325,     0,   225,   255,     0,   224,
     254,     0,   208,   211,     0,   209,   213,   201,   204,   217,
     221,   190,    24,     0,    29,     0,     0,   335,   233,     0,
     239,   290,   336,     0,   251,     0,   252,     0,     0,     0,
       0,   262,   260,   223,    25,   249,     0,     0,   244,   246,
       0,   229,     0,   327,     0,     0,     0,   248,   330,   258,
     328,     0,     0,     0,     0,     0,   257,   322,   321,   323,
     324
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -520,  -520,  -520,  -520,  -520,  -520,  -520,  -397,  -520,   292,
    -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,   263,  -520,
    -520,  -520,  -520,   476,   478,  -520,   425,  -520,  -270,   410,
     420,  -259,  -520,   -71,  -150,   532,  -520,   385,  -520,  -520,
    -151,  -520,  -180,  -218,  -520,  -520,  -520,  -520,   299,  -520,
    -520,  -520,   336,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
     334,   300,   265,   266,  -520,  -520,  -520,  -143,  -520,  -520,
    -520,   519,  -520,  -520,  -520,   -22,   545,  -520,  -520,   230,
      -3,  -520,  -520,  -520,   364,   326,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -425,
    -520,  -499,  -520,  -520,  -520,  -520,   178,  -520,  -520,  -520,
    -519,  -520,   108,  -520,  -520,  -492,  -431,  -429,  -520,  -520,
      -2,  -520,  -520,  -520,  -520,  -520,  -520,   240,  -520,  -520,
    -146,  -520,  -458,  -520,  -520,    45,   661,  -520,  -520,  -520,
    -520,  -520,   606,   513,  -520,  -392,  -520,  -516,  -520,  -104,
     144,  -120,  -520,  -520,  -335,  -520,  -520,   424,  -520,  -520,
    -520,  -520,  -520,  -520,  -520,   597,   777,  -179,  -520,  -520,
    -520,   623,   -78,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -278,  -520,  -520,  -194,   -25,   -84,   -41,    51
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,     4,     8,   373,   406,   407,   408,
     443,   488,   519,   562,   444,   445,   480,   446,   409,     6,
      12,   190,   231,   232,   233,   290,   295,   301,   350,   355,
     364,   296,   400,   234,   235,   434,   341,   342,   237,   343,
     238,   344,   239,   240,   346,   345,   388,   453,   390,   391,
     426,   455,   392,   454,   432,   470,   529,   241,   393,   394,
     429,   461,   462,   463,   464,   465,   466,   186,   273,   269,
     272,   159,   262,   267,   264,   160,   187,   268,   219,   526,
     527,    13,   402,   403,   435,   436,   513,   547,   550,   597,
     647,   599,   594,   642,   645,   555,   601,   602,   649,   514,
     590,   515,   437,   536,   659,   583,   506,   581,   570,   574,
     571,   656,   572,   676,   476,   516,   628,   629,   587,   586,
     121,   113,    90,    93,    61,    95,    96,    92,   122,   106,
      27,    97,   573,   582,    47,   161,    87,   137,   136,   138,
     139,   140,   194,   117,   175,   633,   691,   634,   657,   126,
     621,   151,   206,   152,   382,   452,   383,   179,   205,   256,
     374,   494,    32,   103,   104,    28,   221,    49,    50,    51,
      52,    53,   534,   225,   226,   227,   228,   229,   540,   541,
     495,   257,   308,   559,    29,    30,   118,   112,   153
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,    10,   111,    88,    56,   154,   222,    26,   477,   385,
     449,   258,   575,   507,   188,   188,   577,   557,   189,   105,
     293,   299,   130,   552,    57,   404,    58,   544,   362,   275,
     375,   146,     5,   351,    98,    99,   169,   188,   188,    36,
     209,   210,   665,   510,   482,   370,    91,   544,   595,    94,
     544,   613,    15,    16,    17,   596,   545,   546,   292,   483,
     304,   618,   131,   132,   471,   188,   188,   188,   261,   546,
     266,   203,    88,     9,   353,   271,   279,   546,   299,   545,
     353,   395,   101,   102,   284,   156,   259,     7,   298,   303,
     551,   556,   123,   627,   655,    89,   413,   646,   398,   280,
     281,   660,   650,   643,   135,   648,   405,   224,    19,    20,
      21,    22,    23,   357,   376,    24,   188,   490,   277,   310,
     149,   188,    84,   357,   313,   507,   188,   600,   156,   319,
     277,   278,   603,   353,   610,   116,   358,   183,   389,   178,
     425,   284,   584,   280,   281,   298,     9,   284,   366,    11,
      55,   243,   284,   366,    14,   280,   281,   585,   280,   281,
      54,   147,   156,   605,    89,   619,   607,   291,   297,   302,
     188,    31,   277,   378,   279,   204,   509,   157,   510,   207,
     414,   165,   166,   167,   168,   280,   281,   171,   172,   173,
     174,   458,   459,   460,   664,   635,   666,   280,   281,    33,
     379,   305,   284,   180,   184,    59,   284,   459,   460,   653,
     193,   195,   196,   197,    60,   284,   193,   200,   201,   202,
     157,   352,   356,   278,   279,   297,    55,   352,   365,   100,
     146,   277,   356,   365,   358,   386,   387,   119,    54,   347,
     348,   278,   468,   469,   124,    54,   284,   280,   281,   284,
     127,   284,   183,   125,   157,   638,   280,   281,   128,   277,
     696,   279,   358,   511,   512,   280,   281,   316,   538,   539,
     129,   188,   133,   682,   451,   683,   684,   685,   686,   150,
     352,   284,   396,    84,   280,   281,   399,   162,    15,    16,
      17,   690,   692,   693,   694,   401,    15,    16,    17,   277,
     278,   279,   384,   384,   163,   423,   542,   543,   275,   276,
     277,   278,   279,   188,   548,   549,   498,   553,   554,   184,
     528,   164,   530,   531,   280,   281,   533,   431,    15,    16,
      17,   410,   170,   491,   492,   280,   281,   496,    15,    16,
      17,    36,   420,   176,    19,    20,   129,   254,    23,     9,
     565,    24,    19,    20,    21,    22,    23,   279,   358,    24,
     377,   422,   511,   593,   447,   177,   410,   181,   412,     9,
     608,   191,    62,    63,    64,   548,   626,   192,   532,   208,
     280,   281,   545,   546,    19,    20,    21,    22,    23,   230,
     611,    24,   548,   641,    19,    20,    21,    22,    23,   242,
     410,    24,   553,   644,    15,    16,    17,    25,   120,   568,
     244,   384,    15,    16,    17,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,   665,   510,    83,   245,   479,   530,   531,
     246,   247,   248,   504,   249,   250,   508,   522,   578,   251,
     253,   260,   283,   107,   282,   285,   497,   108,   109,   110,
      19,    20,   129,   254,    23,   287,   289,    24,    19,    20,
      21,    22,    23,   294,   300,    24,   306,    15,    16,    17,
     535,   307,   311,   255,   276,   277,   278,   279,   317,   320,
     349,   428,   615,   354,   532,   359,   361,   363,   368,   460,
     371,   372,   578,   578,   389,   508,   508,   537,   397,   433,
     280,   281,   580,    15,    16,    17,   411,   415,   563,   410,
     416,   417,   410,   566,   418,   421,   578,   424,   448,   457,
     472,   493,   474,    19,    20,    21,    22,    23,   478,   475,
      24,   484,   485,   486,   535,   508,   487,   489,   615,   384,
     499,   521,   500,   471,   503,   508,    25,   134,   508,   620,
      62,    63,    64,   517,   566,   410,   518,   520,   558,    19,
      20,    21,    22,    23,   523,   524,    24,    15,    16,    17,
     662,   578,   525,   578,   578,   578,   578,   560,   561,   588,
     564,   589,    25,   148,   591,   508,   592,   535,   598,   578,
     578,   578,   578,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   543,    36,    83,    62,    63,    64,   542,   604,   606,
     609,   508,   614,    19,    20,    21,    22,    23,   616,   662,
      24,   142,   625,   544,   636,   143,   144,   145,   637,   639,
     640,   651,   535,   546,   652,   654,   658,   663,   545,   610,
      15,    16,    17,   671,   672,   673,   675,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,   674,   695,    83,    15,    16,
      17,   681,   687,   689,   697,   698,    15,    16,    17,   699,
     700,   450,   510,    18,   481,   667,    15,    16,    17,   668,
     669,   670,   286,   367,    34,   288,    19,    20,    21,    22,
      23,   360,   236,    24,   369,   456,   381,   427,   430,   501,
     467,   502,    35,    36,    37,    38,    39,    40,   404,    25,
      62,    63,    64,   274,    19,    20,    21,    22,    23,    41,
      42,    24,    19,    20,    21,    22,    23,   265,   567,    24,
     473,   624,    19,    20,   679,   254,    23,   438,    43,    24,
      44,    45,    46,   579,   141,   185,   309,   199,    62,    63,
      64,   315,   220,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,   680,    48,    83,    15,    16,    17,   419,   223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   405,
     -11,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
       0,    83,    62,    63,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
      19,    20,    21,    22,    23,     0,     0,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,   185,   312,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,     0,    83,     0,   158,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,   115,   182,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     0,     0,    83,     0,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,   115,
     321,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
       0,    83,    84,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,     0,   155,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,     0,    83,
      84,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,     0,   198,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,     0,    83,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,     0,   252,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,     0,    83,    15,    16,    17,     0,
       0,     0,     0,     0,    15,    16,    17,     0,     0,     0,
       0,     0,   661,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,     0,     0,    83,    15,    16,    17,     0,     0,     0,
       0,     0,    15,    16,    17,     0,     0,     0,     0,     0,
     688,     0,    19,    20,    21,    22,    23,     0,     0,    24,
      19,    20,    21,    22,    23,     0,     0,    24,    15,    16,
      17,     0,     0,     0,     0,   185,   318,     0,     0,     0,
       0,     0,     0,   569,   612,     0,     0,     0,     0,     0,
      19,    20,    21,    22,    23,     0,     0,    24,    19,    20,
      21,    22,    23,     0,     0,    24,    62,    63,    64,     0,
       0,     0,     0,    25,   617,     0,     0,     0,     0,     0,
       0,   622,   623,     0,    19,    20,    21,    22,    23,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,   677,   678,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,     0,     0,    83,
      84,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,     0,    86,   630,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,     0,     0,    83,     0,   631,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,     0,     0,   632,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,     0,     0,    83,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,     0,     0,
     115,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
       0,    83,     0,   158,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,     0,     0,   115,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,     0,     0,    83,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,    15,
      16,    17,     0,     0,   314,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,     0,     0,    83,    15,    16,    17,     0,
       0,     0,     0,     0,    15,    16,    17,     0,     0,     0,
       0,   576,     0,     0,     0,    19,    20,    21,    22,    23,
       0,     0,    24,     0,    55,    19,    20,    21,    22,    23,
       0,     0,    24,    15,    16,    17,     0,     0,    25,     0,
       0,    15,    16,    17,     0,     0,     0,     0,    25,     0,
       0,     0,    19,    20,    21,    22,    23,     0,     0,    24,
      19,    20,    21,    22,    23,     0,     0,    24,    15,    16,
      17,     0,     0,     0,     0,   185,     0,     0,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,    19,
      20,    21,    22,    23,     0,     0,    24,    19,    20,    21,
      22,    23,     0,     0,    24,    62,    63,    64,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,     0,     0,
     505,     0,     0,     0,    19,    20,    21,    22,    23,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   569,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,     0,     0,    83,    62,
      63,    64,     0,     0,     0,     0,     0,   439,     0,     0,
     440,   441,   442,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
       0,     0,   339,   340,     0,     0,     0,    15,    16,    17,
       0,     0,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
       0,     0,    83,   211,   212,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    35,    36,   213,   380,     0,    40,   129,   214,   215,
     216,   217,   218,    19,    20,    21,    22,    23,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,     0,   214,   215,   216,   217,
     218,    19,    20,    21,    22,    23,     0,     0,    24,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,     0,     0,   339,   340
};

static const yytype_int16 yycheck[] =
{
      25,     4,    86,    44,    26,   125,   185,     9,   437,   344,
     407,   205,   531,   471,   160,   161,   532,   516,   161,    60,
     238,   239,   100,   515,    26,     1,    28,    40,   298,     6,
     308,   115,     0,   292,    56,    57,   140,   183,   184,    48,
     183,   184,    43,    44,    68,   304,    48,    40,   547,    51,
      40,   570,     3,     4,     5,   547,    69,    70,   238,    83,
     240,   580,   103,   104,    41,   211,   212,   213,   211,    70,
     213,   175,   113,    82,   292,   221,    10,    70,   296,    69,
     298,   351,     4,     5,   234,   126,   206,    82,   239,   240,
     515,   516,    94,   585,   610,    44,   374,   596,   357,    33,
      34,   620,   601,   595,   106,   597,    82,   185,    59,    60,
      61,    62,    63,   293,   308,    66,   262,   452,     8,   262,
     122,   267,    67,   303,   267,   583,   272,   552,   169,   272,
       8,     9,   557,   351,    85,    90,    11,   159,    82,    84,
      84,   291,    68,    33,    34,   296,    82,   297,   299,    82,
      68,   192,   302,   304,     3,    33,    34,    83,    33,    34,
     185,   116,   203,   560,   113,    83,   563,   238,   239,   240,
     316,     3,     8,   316,    10,   177,    42,   126,    44,   181,
     374,   136,   137,   138,   139,    33,    34,   142,   143,   144,
     145,    36,    37,    38,   625,   587,   627,    33,    34,    82,
     320,   242,   352,   152,   159,    82,   356,    37,    38,   606,
     165,   166,   167,   168,    73,   365,   171,   172,   173,   174,
     169,   292,   293,     9,    10,   296,    68,   298,   299,    82,
     314,     8,   303,   304,    11,    82,    83,    83,   263,   280,
     281,     9,    82,    83,    82,   270,   396,    33,    34,   399,
      83,   401,   274,    82,   203,   590,    33,    34,    83,     8,
     689,    10,    11,    81,    82,    33,    34,   269,    59,    60,
      53,   417,    83,   665,   417,   667,   668,   669,   670,    83,
     351,   431,   353,    67,    33,    34,   357,    83,     3,     4,
       5,   683,   684,   685,   686,   366,     3,     4,     5,     8,
       9,    10,   343,   344,    83,   389,    69,    70,     6,     7,
       8,     9,    10,   459,    81,    82,   459,    81,    82,   274,
     499,    82,    29,    30,    33,    34,   505,   398,     3,     4,
       5,   372,    83,   453,   454,    33,    34,   457,     3,     4,
       5,    48,   383,    83,    59,    60,    53,    62,    63,    82,
      83,    66,    59,    60,    61,    62,    63,    10,    11,    66,
     315,   386,    81,    82,   405,    83,   407,    83,    83,    82,
      83,    82,     3,     4,     5,    81,    82,    74,    85,    83,
      33,    34,    69,    70,    59,    60,    61,    62,    63,    82,
     569,    66,    81,    82,    59,    60,    61,    62,    63,    39,
     441,    66,    81,    82,     3,     4,     5,    82,    83,   529,
      83,   452,     3,     4,     5,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    43,    44,    66,    83,   439,    29,    30,
      83,    83,    83,   468,    83,    83,   471,   488,   532,    83,
      83,    83,    82,    84,    83,    82,   458,    88,    89,    90,
      59,    60,    53,    62,    63,    82,    82,    66,    59,    60,
      61,    62,    63,    82,    82,    66,    83,     3,     4,     5,
     505,    63,    83,    82,     7,     8,     9,    10,    83,    82,
      82,    35,   576,    82,    85,    82,    82,    82,    82,    38,
      83,    82,   586,   587,    82,   530,   531,   509,    82,    82,
      33,    34,   534,     3,     4,     5,    83,    83,   520,   560,
      83,    83,   563,   526,    83,    83,   610,    83,    83,    82,
      82,    61,    83,    59,    60,    61,    62,    63,    83,    82,
      66,    83,    83,    83,   569,   570,    83,    83,   632,   590,
      82,    68,    83,    41,    83,   580,    82,    83,   583,   581,
       3,     4,     5,    82,   567,   606,    82,    82,    72,    59,
      60,    61,    62,    63,    83,    83,    66,     3,     4,     5,
     621,   665,    83,   667,   668,   669,   670,    76,    82,    68,
      83,    68,    82,    83,    68,   620,    68,   622,    82,   683,
     684,   685,   686,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    70,    48,    66,     3,     4,     5,    69,    68,    77,
      83,   656,    83,    59,    60,    61,    62,    63,    68,   680,
      66,    84,    83,    40,    83,    88,    89,    90,    83,    83,
      83,    83,   677,    70,    83,    83,    83,    82,    69,    85,
       3,     4,     5,    83,    83,    83,    68,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    83,    82,    66,     3,     4,
       5,    83,    83,    83,    83,    83,     3,     4,     5,    83,
      83,   409,    44,    46,   441,    84,     3,     4,     5,    88,
      89,    90,   236,   303,    29,   237,    59,    60,    61,    62,
      63,   296,   190,    66,   304,   426,   341,   391,   394,   464,
     430,   465,    47,    48,    49,    50,    51,    52,     1,    82,
       3,     4,     5,   224,    59,    60,    61,    62,    63,    64,
      65,    66,    59,    60,    61,    62,    63,   212,   528,    66,
     434,   583,    59,    60,   656,    62,    63,   403,    83,    66,
      85,    86,    87,   533,   113,    82,    83,   171,     3,     4,
       5,   268,   185,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,   657,    25,    66,     3,     4,     5,   383,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      83,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    66,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      59,    60,    61,    62,    63,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    82,    83,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    -1,    66,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    82,    83,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    66,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    82,
      83,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,    83,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,    83,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,    83,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    -1,    66,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,    -1,
      -1,    -1,    83,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    66,     3,     4,     5,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    59,    60,    61,    62,    63,    -1,    -1,    66,
      59,    60,    61,    62,    63,    -1,    -1,    66,     3,     4,
       5,    -1,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      59,    60,    61,    62,    63,    -1,    -1,    66,    59,    60,
      61,    62,    63,    -1,    -1,    66,     3,     4,     5,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    59,    60,    61,    62,    63,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    82,    83,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    -1,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,    82,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    66,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,    -1,    82,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    -1,    66,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,
      82,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    66,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,    -1,    82,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    66,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,    -1,    82,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    -1,    -1,    66,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    59,    60,    61,    62,    63,
      -1,    -1,    66,    -1,    68,    59,    60,    61,    62,    63,
      -1,    -1,    66,     3,     4,     5,    -1,    -1,    82,    -1,
      -1,     3,     4,     5,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    59,    60,    61,    62,    63,    -1,    -1,    66,
      59,    60,    61,    62,    63,    -1,    -1,    66,     3,     4,
       5,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,    62,    63,    -1,    -1,    66,    59,    60,    61,
      62,    63,    -1,    -1,    66,     3,     4,     5,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    59,    60,    61,    62,    63,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      60,    61,    62,    63,    66,    82,   211,   221,   256,   275,
     276,     3,   253,    82,    29,    47,    48,    49,    50,    51,
      52,    64,    65,    83,    85,    86,    87,   225,   257,   258,
     259,   260,   261,   262,   276,    68,   166,   211,   211,    82,
      73,   215,     3,     4,     5,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    66,    67,    68,    82,   227,   278,   279,
     213,   211,   218,   214,   211,   216,   217,   222,   166,   166,
      82,     4,     5,   254,   255,   278,   220,    84,    88,    89,
      90,   277,   278,   212,    68,    82,   226,   234,   277,    83,
      83,   211,   219,   211,    82,    82,   240,    83,    83,    53,
     263,   278,   278,    83,    83,   211,   229,   228,   230,   231,
     232,   227,    84,    88,    89,    90,   277,   226,    83,   211,
      83,   242,   244,   279,   242,    83,   278,   279,    68,   162,
     166,   226,    83,    83,    82,   226,   226,   226,   226,   240,
      83,   226,   226,   226,   226,   235,    83,    83,    84,   248,
     279,    83,    83,   166,   226,    82,   158,   167,   221,   158,
     112,    82,    74,   226,   233,   226,   226,   226,    83,   233,
     226,   226,   226,   240,   211,   249,   243,   211,    83,   158,
     158,    29,    30,    49,    54,    55,    56,    57,    58,   169,
     256,   257,   258,   262,   263,   264,   265,   266,   267,   268,
      82,   113,   114,   115,   124,   125,   126,   129,   131,   133,
     134,   148,    39,   278,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    62,    82,   250,   272,   275,   242,
      83,   158,   163,    82,   165,   167,   158,   164,   168,   160,
      82,   221,   161,   159,   162,     6,     7,     8,     9,    10,
      33,    34,    83,    82,   125,    82,   114,    82,   115,    82,
     116,   124,   133,   134,    82,   117,   122,   124,   131,   134,
      82,   118,   124,   131,   133,   278,    83,    63,   273,    83,
     158,    83,    83,   158,    82,   234,   211,    83,    83,   158,
      82,    83,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    31,
      32,   127,   128,   130,   132,   136,   135,   278,   278,    82,
     119,   122,   124,   134,    82,   120,   124,   133,    11,    82,
     117,    82,   119,    82,   121,   124,   131,   120,    82,   121,
     122,    83,    82,    97,   251,   272,   275,   226,   158,   242,
      83,   128,   245,   247,   278,   245,    82,    83,   137,    82,
     139,   140,   143,   149,   150,   119,   124,    82,   122,   124,
     123,   124,   173,   174,     1,    82,    98,    99,   100,   109,
     278,    83,    83,   272,   275,    83,    83,    83,    83,   248,
     278,    83,   276,   277,    83,    84,   141,   143,    35,   151,
     151,   124,   145,    82,   126,   175,   176,   193,   175,    75,
      78,    79,    80,   101,   105,   106,   108,   278,    83,    98,
     100,   158,   246,   138,   144,   142,   139,    82,    36,    37,
      38,   152,   153,   154,   155,   156,   157,   152,    82,    83,
     146,    41,    82,   176,    83,    82,   205,   208,    83,   211,
     107,   109,    68,    83,    83,    83,    83,    83,   102,    83,
     245,   242,   242,    61,   252,   271,   242,   211,   158,    82,
      83,   153,   154,    83,   276,    82,   197,   223,   276,    42,
      44,    81,    82,   177,   190,   192,   206,    82,    82,   103,
      82,    68,   278,    83,    83,    83,   170,   171,   258,   147,
      29,    30,    85,   258,   263,   276,   194,   211,    59,    60,
     269,   270,    69,    70,    40,    69,    70,   178,    81,    82,
     179,   190,   206,    81,    82,   186,   190,   192,    72,   274,
      76,    82,   104,   211,    83,    83,   171,   170,   242,    82,
     199,   201,   203,   223,   200,   201,    82,   238,   277,   218,
     166,   198,   224,   196,    68,    83,   210,   209,    68,    68,
     191,    68,    68,    82,   183,   192,   206,   180,    82,   182,
     190,   187,   188,   190,    68,    98,    77,    98,    83,    83,
      85,   258,    83,   201,    83,   277,    68,    83,   201,    83,
     166,   241,    82,    83,   197,    83,    82,   206,   207,   208,
      45,    68,    82,   236,   238,   236,    83,    83,   245,    83,
      83,    82,   184,   206,    82,   185,   192,   181,   206,   189,
     192,    83,    83,    98,    83,   238,   202,   239,    83,   195,
     201,    83,   278,    82,   207,    43,   207,    84,    88,    89,
      90,    83,    83,    83,    83,    68,   204,    82,    83,   203,
     241,    83,   236,   236,   236,   236,   236,    83,    83,    83,
     236,   237,   236,   236,   236,    82,   208,    83,    83,    83,
      83
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
     172,   175,   175,   176,   177,   177,   178,   177,   177,   179,
     180,   179,   179,   181,   181,   182,   182,   183,   183,   183,
     184,   184,   185,   185,   186,   186,   187,   186,   188,   188,
     189,   189,   191,   190,   192,   192,   194,   193,   195,   193,
     196,   196,   197,   197,   197,   197,   197,   197,   198,   198,
     199,   199,   200,   201,   201,   202,   202,   203,   203,   204,
     205,   205,   205,   205,   206,   206,   207,   207,   208,   209,
     208,   210,   208,   211,   211,   212,   211,   213,   211,   211,
     211,   214,   211,   215,   211,   211,   216,   211,   217,   211,
     218,   218,   219,   219,   220,   220,   222,   221,   221,   224,
     223,   223,   225,   225,   225,   225,   226,   226,   226,   226,
     226,   226,   227,   228,   227,   229,   227,   230,   227,   231,
     227,   232,   227,   227,   227,   233,   233,   235,   234,   234,
     236,   236,   236,   236,   236,   236,   236,   237,   237,   239,
     238,   238,   240,   240,   240,   241,   241,   242,   242,   243,
     242,   244,   244,   245,   245,   246,   245,   247,   247,   249,
     248,   250,   250,   250,   251,   251,   251,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   275,   275,   275,   275,   275,   275,   276,   276,
     276,   277,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   279
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
      17,     2,     1,     3,     0,     2,     0,     3,     2,     0,
       0,     3,     2,     0,     1,     0,     1,     0,     2,     2,
       0,     1,     0,     1,     0,     2,     0,     3,     0,     1,
       0,     1,     0,     5,     4,     4,     0,     6,     0,     8,
       0,     2,     1,     5,     4,     4,     4,     0,     2,     3,
       2,     1,     1,     1,     4,     0,     2,     1,     5,     1,
       5,     6,     6,     1,     4,     4,     1,     5,     0,     0,
       6,     0,     6,     2,     1,     0,     6,     0,     6,     4,
       4,     0,     5,     0,     5,     5,     0,     8,     0,     8,
       0,     2,     0,     2,     0,     2,     0,     5,     1,     0,
       5,     1,     1,     1,     1,     1,     1,     5,     5,     5,
       5,     1,     1,     0,     6,     0,     6,     0,     6,     0,
       6,     0,     5,     1,     1,     0,     1,     0,     5,     1,
       1,     5,     5,     5,     5,     1,     1,     0,     1,     0,
       5,     1,     0,     2,     2,     0,     2,     0,     1,     0,
       4,     1,     2,     0,     1,     0,     4,     1,     2,     0,
       3,     1,     1,     4,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
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
#line 355 "parser.yy"
       { success = true; line_number = 1; }
#line 2524 "parser.cc"
    break;

  case 3:
#line 355 "parser.yy"
                                                                 {}
#line 2530 "parser.cc"
    break;

  case 4:
#line 357 "parser.yy"
   { success = true; line_number = 1; }
#line 2536 "parser.cc"
    break;

  case 5:
#line 357 "parser.yy"
                                               {}
#line 2542 "parser.cc"
    break;

  case 6:
#line 358 "parser.yy"
  { success = true; line_number = 1; }
#line 2548 "parser.cc"
    break;

  case 7:
#line 358 "parser.yy"
                                                   {}
#line 2554 "parser.cc"
    break;

  case 8:
#line 373 "parser.yy"
            {the_plan=(yyvsp[-1].t_plan); (yyval.t_plan)->name = (char*)(yyvsp[-7].str)->c_str(); (yyval.t_plan)->problem_name = (char*)(yyvsp[-3].str)->c_str();}
#line 2560 "parser.cc"
    break;

  case 9:
#line 377 "parser.yy"
{
    //cout << "Plan" << endl;
    (yyval.t_plan) = new plan(); 
    (yyval.t_plan)->begin = (yyvsp[-1].t_instr);
}
#line 2570 "parser.cc"
    break;

  case 10:
#line 387 "parser.yy"
        {
	  //cout << "Steps" << endl;
	  (yyval.t_instr)= (yyvsp[-1].t_instr);
	  (yyval.t_instr)->next = (yyvsp[0].t_instr);
	}
#line 2580 "parser.cc"
    break;

  case 11:
#line 392 "parser.yy"
                { (yyval.t_instr)= 0; }
#line 2586 "parser.cc"
    break;

  case 12:
#line 396 "parser.yy"
{
  //cout << "Label Step" << endl;
  // mark label as associated with this instruction
  (yyvsp[-1].t_label_symbol)->instr = (yyvsp[0].t_instr);
  (yyval.t_instr) = (yyvsp[0].t_instr); 
}
#line 2597 "parser.cc"
    break;

  case 13:
#line 403 "parser.yy"
{ 
  //cout << "Step" << endl;
  (yyval.t_instr) = (yyvsp[0].t_instr); 
}
#line 2606 "parser.cc"
    break;

  case 14:
#line 409 "parser.yy"
                                 { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2612 "parser.cc"
    break;

  case 15:
#line 410 "parser.yy"
                       { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2618 "parser.cc"
    break;

  case 16:
#line 411 "parser.yy"
                         { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2624 "parser.cc"
    break;

  case 17:
#line 412 "parser.yy"
                         { (yyval.t_instr) = (yyvsp[-1].t_instr); }
#line 2630 "parser.cc"
    break;

  case 18:
#line 414 "parser.yy"
{ 
  //cout << "Done" << endl;
  (yyval.t_instr) = new Done(); 
}
#line 2639 "parser.cc"
    break;

  case 19:
#line 419 "parser.yy"
{
  //cout << "LCPP"<<endl;
  (yyval.t_instr) = new Lcp_Done((yyvsp[-2].num)->double_value(), (yyvsp[-1].num)->double_value());
}
#line 2648 "parser.cc"
    break;

  case 20:
#line 424 "parser.yy"
        {std::cout <<"trouble parsing instruction"<<std::endl;}
#line 2654 "parser.cc"
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
#line 2674 "parser.cc"
    break;

  case 22:
#line 446 "parser.yy"
                  {(yyval.strs)=(yyvsp[-1].strs); (yyvsp[-1].strs)->push_back((yyvsp[0].str));}
#line 2680 "parser.cc"
    break;

  case 23:
#line 447 "parser.yy"
              {(yyval.strs)=new   std::vector<const std::string*>;}
#line 2686 "parser.cc"
    break;

  case 24:
#line 451 "parser.yy"
{
  //cout << "Then" << endl;
  (yyval.t_instr) = (yyvsp[-1].t_instr);
}
#line 2695 "parser.cc"
    break;

  case 25:
#line 458 "parser.yy"
{
  //cout << "Else" << endl;
  (yyval.t_instr) = (yyvsp[-1].t_instr);
}
#line 2704 "parser.cc"
    break;

  case 26:
#line 465 "parser.yy"
{
  //cout << "IfThen" << endl;
  (yyval.t_instr) = new IfThen(formula_bdd(*(yyvsp[-1].formula), false),(yyvsp[0].t_instr));
}
#line 2713 "parser.cc"
    break;

  case 27:
#line 470 "parser.yy"
{
  //cout << "IfThenElse" << endl;
  (yyval.t_instr) = new IfThenElse(formula_bdd(*(yyvsp[-2].formula), false),(yyvsp[-1].t_instr),(yyvsp[0].t_instr));
}
#line 2722 "parser.cc"
    break;

  case 28:
#line 477 "parser.yy"
{
  //cout << "Case" << endl;
  (yyval.t_instr) = new Case((yyvsp[0].t_guards));
}
#line 2731 "parser.cc"
    break;

  case 29:
#line 484 "parser.yy"
{
  //cout << "Guards" << endl;
  (yyval.t_guards) = (yyvsp[-4].t_guards);
  (yyval.t_guards)->push_back(std::make_pair(formula_bdd(*(yyvsp[-2].formula), false),(yyvsp[-1].t_instr)));
}
#line 2741 "parser.cc"
    break;

  case 30:
#line 489 "parser.yy"
              { (yyval.t_guards) = new Guards(); }
#line 2747 "parser.cc"
    break;

  case 31:
#line 494 "parser.yy"
{
  //cout << "Goto" << endl;
  (yyval.t_instr) = new Goto((yyvsp[0].t_label_symbol));
}
#line 2756 "parser.cc"
    break;

  case 32:
#line 501 "parser.yy"
{
  // make a label object
  // put/find in a hash table
  (yyval.t_label_symbol) = new label_symbol(*(yyvsp[0].str));//current_analysis->label_tab.symbol_ref($1);
}
#line 2766 "parser.cc"
    break;

  case 34:
#line 509 "parser.yy"
                                                        {std::cout << "finish definition of domain" << std::endl;}
#line 2772 "parser.cc"
    break;

  case 35:
#line 510 "parser.yy"
                                                         {std::cout << "finish definition of problem" << std::endl;}
#line 2778 "parser.cc"
    break;

  case 36:
#line 517 "parser.yy"
                                            { make_domain((yyvsp[-1].str)); }
#line 2784 "parser.cc"
    break;

  case 83:
#line 590 "parser.yy"
                                   {// cout << "ob tokens"<<endl;
}
#line 2791 "parser.cc"
    break;

  case 84:
#line 591 "parser.yy"
                       {// cout << "done ob tokens"<<endl;
}
#line 2798 "parser.cc"
    break;

  case 91:
#line 610 "parser.yy"
                     { requirements->strips = true; }
#line 2804 "parser.cc"
    break;

  case 92:
#line 611 "parser.yy"
                     { requirements->typing = true; }
#line 2810 "parser.cc"
    break;

  case 93:
#line 613 "parser.yy"
                { requirements->negative_preconditions = true; }
#line 2816 "parser.cc"
    break;

  case 94:
#line 615 "parser.yy"
                { requirements->disjunctive_preconditions = true; }
#line 2822 "parser.cc"
    break;

  case 95:
#line 616 "parser.yy"
                       { requirements->equality = true; }
#line 2828 "parser.cc"
    break;

  case 96:
#line 618 "parser.yy"
                { requirements->existential_preconditions = true; }
#line 2834 "parser.cc"
    break;

  case 97:
#line 620 "parser.yy"
                { requirements->universal_preconditions = true; }
#line 2840 "parser.cc"
    break;

  case 98:
#line 622 "parser.yy"
                { requirements->quantified_preconditions(); }
#line 2846 "parser.cc"
    break;

  case 99:
#line 623 "parser.yy"
                                  { requirements->conditional_effects = true; }
#line 2852 "parser.cc"
    break;

  case 100:
#line 624 "parser.yy"
                                     { requirements->probabilistic = true; }
#line 2858 "parser.cc"
    break;

  case 101:
#line 625 "parser.yy"
                                         { // 开启不确定性 :non-deterministic
				requirements->non_deterministic = true;
				requirements->probabilistic_effects = true;// 这里还是开启了概率Effect
				requirements->probabilistic = false;
				}
#line 2868 "parser.cc"
    break;

  case 102:
#line 630 "parser.yy"
                      { requirements->fluents = true; }
#line 2874 "parser.cc"
    break;

  case 103:
#line 631 "parser.yy"
                  { requirements->adl(); }
#line 2880 "parser.cc"
    break;

  case 104:
#line 633 "parser.yy"
                { throw Exception("`:durative-actions' not supported"); }
#line 2886 "parser.cc"
    break;

  case 105:
#line 635 "parser.yy"
                { throw Exception("`:duration-inequalities' not supported"); }
#line 2892 "parser.cc"
    break;

  case 106:
#line 637 "parser.yy"
                { throw Exception("`:continuous-effects' not supported"); }
#line 2898 "parser.cc"
    break;

  case 107:
#line 639 "parser.yy"
                {
					requirements->probabilistic_effects = true;
					requirements->probabilistic = true;
					requirements->non_deterministic = false;// 关闭不确定性
					goal_prob_function = domain->functions().add_function("goal-probability");
				}
#line 2909 "parser.cc"
    break;

  case 108:
#line 646 "parser.yy"
                {
					requirements->rewards = true;
					reward_function = domain->functions().add_function("reward");
				}
#line 2918 "parser.cc"
    break;

  case 109:
#line 651 "parser.yy"
                {
					requirements->probabilistic_effects = true;
					requirements->rewards = true;
					goal_prob_function = domain->functions().add_function("goal-probability");
					reward_function = domain->functions().add_function("reward");
				}
#line 2929 "parser.cc"
    break;

  case 110:
#line 666 "parser.yy"
                      { require_typing(); name_kind = TYPE_KIND; }
#line 2935 "parser.cc"
    break;

  case 111:
#line 667 "parser.yy"
                              { name_kind = VOID_KIND; }
#line 2941 "parser.cc"
    break;

  case 112:
#line 670 "parser.yy"
                              { name_kind = CONSTANT_KIND; }
#line 2947 "parser.cc"
    break;

  case 113:
#line 671 "parser.yy"
                  { name_kind = VOID_KIND; }
#line 2953 "parser.cc"
    break;

  case 115:
#line 677 "parser.yy"
                              { require_fluents(); }
#line 2959 "parser.cc"
    break;

  case 119:
#line 688 "parser.yy"
                               { make_predicate((yyvsp[0].str)); }
#line 2965 "parser.cc"
    break;

  case 120:
#line 689 "parser.yy"
                   { parsing_predicate = false; }
#line 2971 "parser.cc"
    break;

  case 126:
#line 701 "parser.yy"
                         { require_typing(); }
#line 2977 "parser.cc"
    break;

  case 128:
#line 704 "parser.yy"
                             { make_function((yyvsp[0].str)); }
#line 2983 "parser.cc"
    break;

  case 129:
#line 705 "parser.yy"
                  { parsing_function = false; }
#line 2989 "parser.cc"
    break;

  case 132:
#line 713 "parser.yy"
                                        { make_predicate((yyvsp[0].str));
					// make_observable($2); parsing_obs_token = false;
					}
#line 2997 "parser.cc"
    break;

  case 133:
#line 716 "parser.yy"
                                        { //parsing_obs_token = false;
 					  parsing_predicate = false; 
					}
#line 3005 "parser.cc"
    break;

  case 134:
#line 725 "parser.yy"
                             {// cout << *$3<<endl;
               make_action((yyvsp[0].str));  }
#line 3012 "parser.cc"
    break;

  case 135:
#line 727 "parser.yy"
                                          { add_action(); }
#line 3018 "parser.cc"
    break;

  case 136:
#line 728 "parser.yy"
                            {
               make_action((yyvsp[0].str));  }
#line 3025 "parser.cc"
    break;

  case 137:
#line 730 "parser.yy"
                                          { add_event(); }
#line 3031 "parser.cc"
    break;

  case 147:
#line 753 "parser.yy"
                                    { action->set_precondition(*(yyvsp[0].formula)); }
#line 3037 "parser.cc"
    break;

  case 148:
#line 756 "parser.yy"
                            { action->set_effect(*(yyvsp[0].effect)); }
#line 3043 "parser.cc"
    break;

  case 149:
#line 762 "parser.yy"
                                                         {action->set_observation(*(yyvsp[-1].observation_defs));}
#line 3049 "parser.cc"
    break;

  case 150:
#line 763 "parser.yy"
                                                     {action->set_observation(*(yyvsp[-1].observation_defs)); // cout << "parse ob"<<endl;
}
#line 3056 "parser.cc"
    break;

  case 152:
#line 770 "parser.yy"
                                       { (yyval.effect) = (yyvsp[-1].ceffect); }
#line 3062 "parser.cc"
    break;

  case 153:
#line 771 "parser.yy"
                         { prepare_forall_effect(); }
#line 3068 "parser.cc"
    break;

  case 154:
#line 772 "parser.yy"
                                { (yyval.effect) = make_forall_effect(*(yyvsp[-1].effect)); }
#line 3074 "parser.cc"
    break;

  case 155:
#line 773 "parser.yy"
                       { require_conditional_effects(); }
#line 3080 "parser.cc"
    break;

  case 156:
#line 774 "parser.yy"
                                { (yyval.effect) = &ConditionalEffect::make(*(yyvsp[-2].formula), *(yyvsp[-1].effect)); }
#line 3086 "parser.cc"
    break;

  case 157:
#line 775 "parser.yy"
                                              { if((yyvsp[-1].peffect)->size() ==1 && (yyvsp[-1].peffect)->probability(0) == 1.0){ (yyval.effect)=&(yyvsp[-1].peffect)->effect(0);} else (yyval.effect) = (yyvsp[-1].peffect); }
#line 3092 "parser.cc"
    break;

  case 158:
#line 776 "parser.yy"
                                       { (yyval.effect) = (yyvsp[-1].peffect); }
#line 3098 "parser.cc"
    break;

  case 159:
#line 777 "parser.yy"
                                 { (yyval.effect) = (yyvsp[-1].peffect); }
#line 3104 "parser.cc"
    break;

  case 160:
#line 778 "parser.yy"
                                           { (yyval.effect) = (yyvsp[-1].peffect); }
#line 3110 "parser.cc"
    break;

  case 161:
#line 781 "parser.yy"
                           { (yyval.ceffect) = new ConjunctiveEffect(); }
#line 3116 "parser.cc"
    break;

  case 162:
#line 782 "parser.yy"
                                        { (yyval.ceffect) = (yyvsp[-1].ceffect); (yyval.ceffect)->add_conjunct(*(yyvsp[0].effect)); }
#line 3122 "parser.cc"
    break;

  case 163:
#line 786 "parser.yy"
               {
 		(yyval.peffect) = new ProbabilisticEffect();
 		add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect));
 	      }
#line 3131 "parser.cc"
    break;

  case 164:
#line 791 "parser.yy"
              { (yyval.peffect) = (yyvsp[-2].peffect); add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect)); }
#line 3137 "parser.cc"
    break;

  case 165:
#line 793 "parser.yy"
              {
                //$1->print(cout,problem->domain().functions(),problem->terms()); 
		(yyval.peffect) = new ProbabilisticEffect();
		add_feffect_outcome(*(yyval.peffect), (yyvsp[-1].expr), *(yyvsp[0].effect));
	      }
#line 3147 "parser.cc"
    break;

  case 166:
#line 800 "parser.yy"
          {
	    //$2->print(cout,problem->domain().functions(),problem->terms()); 
	   (yyval.peffect) = (yyvsp[-2].peffect); add_feffect_outcome(*(yyval.peffect), (yyvsp[-1].expr), *(yyvsp[0].effect)); }
#line 3155 "parser.cc"
    break;

  case 167:
#line 806 "parser.yy"
             { (yyval.peffect) = (yyvsp[-1].peffect); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3161 "parser.cc"
    break;

  case 168:
#line 808 "parser.yy"
{ (yyval.peffect) = new ProbabilisticEffect(); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3167 "parser.cc"
    break;

  case 169:
#line 812 "parser.yy"
             { (yyval.peffect) = (yyvsp[-1].peffect); add_effect_outcome(*(yyval.peffect), new Rational(-3.0), *(yyvsp[0].effect)); }
#line 3173 "parser.cc"
    break;

  case 170:
#line 814 "parser.yy"
                         { (yyval.peffect) = new ProbabilisticEffect(); add_effect_outcome(*(yyval.peffect), new Rational(-3.0), *(yyvsp[0].effect)); }
#line 3179 "parser.cc"
    break;

  case 171:
#line 818 "parser.yy"
               { (yyval.peffect) = new ProbabilisticEffect(); 
                 add_effect_outcome(*(yyval.peffect), new Rational(-2.0), *(yyvsp[0].effect)); }
#line 3186 "parser.cc"
    break;

  case 173:
#line 827 "parser.yy"
                               {std::cout << "make_add_effect" << std::endl; (yyval.effect) = make_add_effect(*(yyvsp[0].atom)); }
#line 3192 "parser.cc"
    break;

  case 174:
#line 828 "parser.yy"
                                           {std::cout << "make_delete_effect" << std::endl; (yyval.effect) = make_delete_effect(*(yyvsp[-1].atom)); }
#line 3198 "parser.cc"
    break;

  case 175:
#line 829 "parser.yy"
                         { effect_fluent = true; }
#line 3204 "parser.cc"
    break;

  case 176:
#line 830 "parser.yy"
             { (yyval.effect) = make_assignment_effect((yyvsp[-4].setop), *(yyvsp[-2].appl), *(yyvsp[-1].expr)); }
#line 3210 "parser.cc"
    break;

  case 177:
#line 833 "parser.yy"
                   { (yyval.setop) = Assignment::ASSIGN_OP; }
#line 3216 "parser.cc"
    break;

  case 178:
#line 834 "parser.yy"
                     { (yyval.setop) = Assignment::SCALE_UP_OP; }
#line 3222 "parser.cc"
    break;

  case 179:
#line 835 "parser.yy"
                       { (yyval.setop) = Assignment::SCALE_DOWN_OP; }
#line 3228 "parser.cc"
    break;

  case 180:
#line 836 "parser.yy"
                     { (yyval.setop) = Assignment::INCREASE_OP; }
#line 3234 "parser.cc"
    break;

  case 181:
#line 837 "parser.yy"
                     { (yyval.setop) = Assignment::DECREASE_OP; }
#line 3240 "parser.cc"
    break;

  case 182:
#line 843 "parser.yy"
                                                {(yyval.observation_defs)->add_entry((yyvsp[0].observation)); }
#line 3246 "parser.cc"
    break;

  case 183:
#line 844 "parser.yy"
                               {(yyval.observation_defs) = new Observation(); (yyval.observation_defs)->add_entry((yyvsp[0].observation)); }
#line 3252 "parser.cc"
    break;

  case 184:
#line 847 "parser.yy"
                                                        {(yyval.observation) = make_observation(*(yyvsp[-3].formula), *(yyvsp[-2].num), *(yyvsp[-1].num));}
#line 3258 "parser.cc"
    break;

  case 185:
#line 848 "parser.yy"
                                        {(yyval.observation) = make_observation(*(yyvsp[-3].formula), *(yyvsp[-2].formula) , *(yyvsp[-1].num)); }
#line 3264 "parser.cc"
    break;

  case 186:
#line 849 "parser.yy"
                                                        {(yyval.observation) = make_observation(*(yyvsp[-5].formula), *(yyvsp[-2].peffect)); (yyvsp[-2].peffect)->setObservation();}
#line 3270 "parser.cc"
    break;

  case 187:
#line 866 "parser.yy"
                { make_problem((yyvsp[-5].str), (yyvsp[-1].str)); }
#line 3276 "parser.cc"
    break;

  case 188:
#line 867 "parser.yy"
                { problem->instantiate_actions(); problem->instantiate_events(); delete requirements; }
#line 3282 "parser.cc"
    break;

  case 189:
#line 869 "parser.yy"
               { make_problem((yyvsp[-5].str), (yyvsp[-1].str)); }
#line 3288 "parser.cc"
    break;

  case 190:
#line 870 "parser.yy"
               { problem->instantiate_actions();  problem->instantiate_events(); problem->set_plan_time(*(yyvsp[-1].num));
	         delete (yyvsp[-1].num); delete requirements; }
#line 3295 "parser.cc"
    break;

  case 196:
#line 887 "parser.yy"
                               {std::cout << "finish object 1" << std::endl;}
#line 3301 "parser.cc"
    break;

  case 200:
#line 893 "parser.yy"
                               {std::cout << "finish object 2" << std::endl;}
#line 3307 "parser.cc"
    break;

  case 206:
#line 902 "parser.yy"
                               {std::cout << "finish object 3" << std::endl;}
#line 3313 "parser.cc"
    break;

  case 207:
#line 906 "parser.yy"
                              {std::cout << "empty problem_body_o" << std::endl;}
#line 3319 "parser.cc"
    break;

  case 210:
#line 911 "parser.yy"
                              {std::cout << "empty problem_body_oh" << std::endl;}
#line 3325 "parser.cc"
    break;

  case 216:
#line 922 "parser.yy"
                               {std::cout << "finish object" << std::endl;}
#line 3331 "parser.cc"
    break;

  case 219:
#line 926 "parser.yy"
                              {std::cout << "finish object" << std::endl;}
#line 3337 "parser.cc"
    break;

  case 222:
#line 934 "parser.yy"
                          { name_kind = OBJECT_KIND; std::cout << "start object" << std::endl;}
#line 3343 "parser.cc"
    break;

  case 223:
#line 935 "parser.yy"
                { name_kind = VOID_KIND; std::cout << "end object" << std::endl;}
#line 3349 "parser.cc"
    break;

  case 224:
#line 938 "parser.yy"
                                      {problem->set_plan_horizon(*(yyvsp[-1].num));}
#line 3355 "parser.cc"
    break;

  case 225:
#line 939 "parser.yy"
                                               {problem->set_plan_horizon(*(yyvsp[-1].num));}
#line 3361 "parser.cc"
    break;

  case 226:
#line 943 "parser.yy"
                             {std::cout << "init_element " << std::endl;}
#line 3367 "parser.cc"
    break;

  case 228:
#line 945 "parser.yy"
       { std::cout << "init and conjuncts\n"; problem->set_init_formula(*(yyvsp[-1].conj)); get_init_elts();}
#line 3373 "parser.cc"
    break;

  case 230:
#line 950 "parser.yy"
                            {std::cout << "empty init" << std::endl;}
#line 3379 "parser.cc"
    break;

  case 231:
#line 951 "parser.yy"
                                           {std::cout << "init element" << std::endl;}
#line 3385 "parser.cc"
    break;

  case 232:
#line 955 "parser.yy"
                                {
					std::cout << "atom formula" << std::endl; 
					problem->add_init_atom(*(yyvsp[0].atom));  problem->add_init_effect(*(new AddEffect(*(yyvsp[0].atom))));}
#line 3393 "parser.cc"
    break;

  case 233:
#line 959 "parser.yy"
                 { problem->add_init_value(*(yyvsp[-2].appl), *(yyvsp[-1].num)); delete (yyvsp[-1].num); }
#line 3399 "parser.cc"
    break;

  case 234:
#line 961 "parser.yy"
                 { problem->add_init_effect(*(yyvsp[-1].peffect)); }
#line 3405 "parser.cc"
    break;

  case 235:
#line 963 "parser.yy"
                                { std::cout << "oneof_init" << std::endl; problem->add_init_effect(*(yyvsp[-1].peffect)); }
#line 3411 "parser.cc"
    break;

  case 236:
#line 964 "parser.yy"
                                            { problem->add_init_effect(*(yyvsp[-1].peffect)); }
#line 3417 "parser.cc"
    break;

  case 238:
#line 976 "parser.yy"
               {
		 (yyval.peffect) = new ProbabilisticEffect();
		 add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect));
	       }
#line 3426 "parser.cc"
    break;

  case 239:
#line 981 "parser.yy"
           { (yyval.peffect) = (yyvsp[-2].peffect); add_effect_outcome(*(yyval.peffect), (yyvsp[-1].num), *(yyvsp[0].effect)); }
#line 3432 "parser.cc"
    break;

  case 240:
#line 985 "parser.yy"
             { (yyval.peffect) = (yyvsp[-1].peffect); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3438 "parser.cc"
    break;

  case 241:
#line 987 "parser.yy"
{ (yyval.peffect) = new ProbabilisticEffect(); add_effect_outcome(*(yyval.peffect), new Rational(-1.0), *(yyvsp[0].effect)); }
#line 3444 "parser.cc"
    break;

  case 242:
#line 991 "parser.yy"
               { (yyval.peffect) = new ProbabilisticEffect(); 
                 add_effect_outcome(*(yyval.peffect), new Rational(-2.0), *(yyvsp[0].effect)); }
#line 3451 "parser.cc"
    break;

  case 244:
#line 996 "parser.yy"
                                    { (yyval.effect) = (yyvsp[-1].ceffect); }
#line 3457 "parser.cc"
    break;

  case 245:
#line 999 "parser.yy"
                        { (yyval.ceffect) = new ConjunctiveEffect(); }
#line 3463 "parser.cc"
    break;

  case 246:
#line 1000 "parser.yy"
                               { (yyval.ceffect) = (yyvsp[-1].ceffect); (yyval.ceffect)->add_conjunct(*(yyvsp[0].effect)); }
#line 3469 "parser.cc"
    break;

  case 247:
#line 1003 "parser.yy"
                               { (yyval.effect) = make_add_effect(*(yyvsp[0].atom)); }
#line 3475 "parser.cc"
    break;

  case 248:
#line 1005 "parser.yy"
             { (yyval.effect) = make_assignment_effect(Assignment::ASSIGN_OP, *(yyvsp[-2].appl), *(yyvsp[-1].expr)); }
#line 3481 "parser.cc"
    break;

  case 249:
#line 1008 "parser.yy"
               { (yyval.expr) = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3487 "parser.cc"
    break;

  case 250:
#line 1011 "parser.yy"
                                             { problem->set_goal(*(yyvsp[-2].formula), true); }
#line 3493 "parser.cc"
    break;

  case 251:
#line 1012 "parser.yy"
                                                    { problem->set_goal(*(yyvsp[-3].formula), *(yyvsp[-2].num)); delete (yyvsp[-2].num); }
#line 3499 "parser.cc"
    break;

  case 252:
#line 1013 "parser.yy"
                                                      { problem->set_goal(*(yyvsp[-3].formula), true); }
#line 3505 "parser.cc"
    break;

  case 254:
#line 1017 "parser.yy"
                               {set_discount(*(yyvsp[-1].num));}
#line 3511 "parser.cc"
    break;

  case 255:
#line 1018 "parser.yy"
                                        {set_discount(*(yyvsp[-1].num));}
#line 3517 "parser.cc"
    break;

  case 257:
#line 1023 "parser.yy"
                { set_goal_reward(*(yyvsp[-2].expr)); }
#line 3523 "parser.cc"
    break;

  case 258:
#line 1026 "parser.yy"
                          { std::cout << "Set default metric()" << std::endl; set_default_metric(); }
#line 3529 "parser.cc"
    break;

  case 259:
#line 1027 "parser.yy"
                                  { metric_fluent = true; }
#line 3535 "parser.cc"
    break;

  case 260:
#line 1028 "parser.yy"
                { problem->set_metric(*(yyvsp[-1].expr)); metric_fluent = false; }
#line 3541 "parser.cc"
    break;

  case 261:
#line 1029 "parser.yy"
                                  { metric_fluent = true; }
#line 3547 "parser.cc"
    break;

  case 262:
#line 1030 "parser.yy"
                { problem->set_metric(*(yyvsp[-1].expr), true); metric_fluent = false; }
#line 3553 "parser.cc"
    break;

  case 263:
#line 1038 "parser.yy"
                        {std::cout << "empty formula: line " << line_number << std::endl;}
#line 3559 "parser.cc"
    break;

  case 264:
#line 1039 "parser.yy"
                                      { (yyval.formula) = (yyvsp[0].atom); }
#line 3565 "parser.cc"
    break;

  case 265:
#line 1041 "parser.yy"
            { first_eq_term = eq_term; first_eq_expr = eq_expr; }
#line 3571 "parser.cc"
    break;

  case 266:
#line 1042 "parser.yy"
                              { (yyval.formula) = make_equality(); }
#line 3577 "parser.cc"
    break;

  case 267:
#line 1043 "parser.yy"
                          { require_fluents(); }
#line 3583 "parser.cc"
    break;

  case 268:
#line 1044 "parser.yy"
            { (yyval.formula) = new Comparison((yyvsp[-4].comp), *(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3589 "parser.cc"
    break;

  case 269:
#line 1045 "parser.yy"
                              { (yyval.formula) = make_negation(*(yyvsp[-1].formula)); }
#line 3595 "parser.cc"
    break;

  case 270:
#line 1046 "parser.yy"
                                { (yyval.formula) = (yyvsp[-1].conj); }
#line 3601 "parser.cc"
    break;

  case 271:
#line 1047 "parser.yy"
                 { require_disjunction(); }
#line 3607 "parser.cc"
    break;

  case 272:
#line 1047 "parser.yy"
                                                          { (yyval.formula) = (yyvsp[-1].disj); }
#line 3613 "parser.cc"
    break;

  case 273:
#line 1048 "parser.yy"
                    { require_disjunction(); }
#line 3619 "parser.cc"
    break;

  case 274:
#line 1048 "parser.yy"
                                                                   { (yyval.formula) = (yyvsp[-1].odisj); }
#line 3625 "parser.cc"
    break;

  case 275:
#line 1049 "parser.yy"
                                        { (yyval.formula) = make_implication(*(yyvsp[-2].formula), *(yyvsp[-1].formula)); }
#line 3631 "parser.cc"
    break;

  case 276:
#line 1050 "parser.yy"
                     { prepare_exists(); }
#line 3637 "parser.cc"
    break;

  case 277:
#line 1051 "parser.yy"
            { (yyval.formula) = make_exists(*(yyvsp[-1].formula)); }
#line 3643 "parser.cc"
    break;

  case 278:
#line 1052 "parser.yy"
                     { prepare_forall(); }
#line 3649 "parser.cc"
    break;

  case 279:
#line 1053 "parser.yy"
            { (yyval.formula) = make_forall(*(yyvsp[-1].formula)); }
#line 3655 "parser.cc"
    break;

  case 280:
#line 1056 "parser.yy"
                        { (yyval.conj) = new Conjunction(); }
#line 3661 "parser.cc"
    break;

  case 281:
#line 1057 "parser.yy"
                              { (yyval.conj)->add_conjunct(*(yyvsp[0].formula)); }
#line 3667 "parser.cc"
    break;

  case 282:
#line 1060 "parser.yy"
                        { (yyval.disj) = new Disjunction(); }
#line 3673 "parser.cc"
    break;

  case 283:
#line 1061 "parser.yy"
                              { (yyval.disj)->add_disjunct(*(yyvsp[0].formula)); }
#line 3679 "parser.cc"
    break;

  case 284:
#line 1063 "parser.yy"
                              { (yyval.odisj) = new OneOfDisjunction(); }
#line 3685 "parser.cc"
    break;

  case 285:
#line 1064 "parser.yy"
                                    { (yyval.odisj)->add_oneof_disjunct(*(yyvsp[0].formula)); }
#line 3691 "parser.cc"
    break;

  case 286:
#line 1067 "parser.yy"
                                    { prepare_atom((yyvsp[0].str)); }
#line 3697 "parser.cc"
    break;

  case 287:
#line 1068 "parser.yy"
                        { (yyval.atom) = make_atom(); }
#line 3703 "parser.cc"
    break;

  case 288:
#line 1069 "parser.yy"
                                { prepare_atom((yyvsp[0].str)); (yyval.atom) = make_atom(); }
#line 3709 "parser.cc"
    break;

  case 289:
#line 1072 "parser.yy"
                                    { prepare_atom((yyvsp[0].str)); }
#line 3715 "parser.cc"
    break;

  case 290:
#line 1073 "parser.yy"
                        { (yyval.atom) = make_atom(); }
#line 3721 "parser.cc"
    break;

  case 291:
#line 1074 "parser.yy"
                                { prepare_atom((yyvsp[0].str)); (yyval.atom) = make_atom(); }
#line 3727 "parser.cc"
    break;

  case 292:
#line 1077 "parser.yy"
                  { (yyval.comp) = Comparison::LT_CMP; }
#line 3733 "parser.cc"
    break;

  case 293:
#line 1078 "parser.yy"
                 { (yyval.comp) = Comparison::LE_CMP; }
#line 3739 "parser.cc"
    break;

  case 294:
#line 1079 "parser.yy"
                 { (yyval.comp) = Comparison::GE_CMP; }
#line 3745 "parser.cc"
    break;

  case 295:
#line 1080 "parser.yy"
                  {(yyval.comp) = Comparison::GT_CMP; }
#line 3751 "parser.cc"
    break;

  case 296:
#line 1087 "parser.yy"
               { (yyval.expr) = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3757 "parser.cc"
    break;

  case 297:
#line 1088 "parser.yy"
                                { (yyval.expr) = new Addition(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3763 "parser.cc"
    break;

  case 298:
#line 1089 "parser.yy"
                                    { (yyval.expr) = make_subtraction(*(yyvsp[-2].expr), (yyvsp[-1].expr)); }
#line 3769 "parser.cc"
    break;

  case 299:
#line 1090 "parser.yy"
                                { (yyval.expr) = new Multiplication(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3775 "parser.cc"
    break;

  case 300:
#line 1091 "parser.yy"
                                { (yyval.expr) = new Division(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3781 "parser.cc"
    break;

  case 301:
#line 1092 "parser.yy"
               { (yyval.expr) = (yyvsp[0].appl); }
#line 3787 "parser.cc"
    break;

  case 302:
#line 1096 "parser.yy"
                  { require_fluents(); eq_expr = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3793 "parser.cc"
    break;

  case 303:
#line 1097 "parser.yy"
                        { require_fluents(); }
#line 3799 "parser.cc"
    break;

  case 304:
#line 1098 "parser.yy"
                  { eq_expr = new Addition(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3805 "parser.cc"
    break;

  case 305:
#line 1099 "parser.yy"
                        { require_fluents(); }
#line 3811 "parser.cc"
    break;

  case 306:
#line 1100 "parser.yy"
                  { eq_expr = make_subtraction(*(yyvsp[-2].expr), (yyvsp[-1].expr)); }
#line 3817 "parser.cc"
    break;

  case 307:
#line 1101 "parser.yy"
                        { require_fluents(); }
#line 3823 "parser.cc"
    break;

  case 308:
#line 1102 "parser.yy"
                  { eq_expr = new Multiplication(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3829 "parser.cc"
    break;

  case 309:
#line 1103 "parser.yy"
                        { require_fluents(); }
#line 3835 "parser.cc"
    break;

  case 310:
#line 1104 "parser.yy"
                  { eq_expr = new Division(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3841 "parser.cc"
    break;

  case 311:
#line 1105 "parser.yy"
                             { require_fluents(); prepare_application((yyvsp[0].str)); }
#line 3847 "parser.cc"
    break;

  case 312:
#line 1106 "parser.yy"
                            { eq_expr = make_application(); }
#line 3853 "parser.cc"
    break;

  case 313:
#line 1107 "parser.yy"
                     { make_eq_name((yyvsp[0].str)); }
#line 3859 "parser.cc"
    break;

  case 314:
#line 1108 "parser.yy"
                         { eq_term = make_term((yyvsp[0].str)); eq_expr = NULL; }
#line 3865 "parser.cc"
    break;

  case 315:
#line 1111 "parser.yy"
                        { (yyval.expr) = NULL; }
#line 3871 "parser.cc"
    break;

  case 317:
#line 1115 "parser.yy"
                      { prepare_application((yyvsp[0].str)); }
#line 3877 "parser.cc"
    break;

  case 318:
#line 1116 "parser.yy"
           { (yyval.appl) = make_application(); }
#line 3883 "parser.cc"
    break;

  case 319:
#line 1117 "parser.yy"
                  { prepare_application((yyvsp[0].str)); (yyval.appl) = make_application(); }
#line 3889 "parser.cc"
    break;

  case 320:
#line 1120 "parser.yy"
                      { (yyval.expr) = new Value(*(yyvsp[0].num)); delete (yyvsp[0].num); }
#line 3895 "parser.cc"
    break;

  case 321:
#line 1122 "parser.yy"
                 { (yyval.expr) = new Addition(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3901 "parser.cc"
    break;

  case 322:
#line 1124 "parser.yy"
                 { (yyval.expr) = make_subtraction(*(yyvsp[-2].expr), (yyvsp[-1].expr)); }
#line 3907 "parser.cc"
    break;

  case 323:
#line 1126 "parser.yy"
                 { (yyval.expr) = new Multiplication(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3913 "parser.cc"
    break;

  case 324:
#line 1128 "parser.yy"
                 { (yyval.expr) = new Division(*(yyvsp[-2].expr), *(yyvsp[-1].expr)); }
#line 3919 "parser.cc"
    break;

  case 325:
#line 1129 "parser.yy"
                             { (yyval.expr) = (yyvsp[0].appl); }
#line 3925 "parser.cc"
    break;

  case 326:
#line 1131 "parser.yy"
                 { prepare_application((yyvsp[0].str)); (yyval.expr) = make_application(); }
#line 3931 "parser.cc"
    break;

  case 327:
#line 1134 "parser.yy"
                               { (yyval.expr) = NULL; }
#line 3937 "parser.cc"
    break;

  case 329:
#line 1138 "parser.yy"
                             { prepare_application((yyvsp[0].str)); }
#line 3943 "parser.cc"
    break;

  case 330:
#line 1139 "parser.yy"
                  { (yyval.appl) = make_application(); }
#line 3949 "parser.cc"
    break;

  case 331:
#line 1141 "parser.yy"
                    { prepare_application((yyvsp[0].str)); (yyval.appl) = make_application(); }
#line 3955 "parser.cc"
    break;

  case 333:
#line 1149 "parser.yy"
                   { add_term((yyvsp[0].str)); }
#line 3961 "parser.cc"
    break;

  case 334:
#line 1150 "parser.yy"
                       { add_term((yyvsp[0].str)); }
#line 3967 "parser.cc"
    break;

  case 336:
#line 1154 "parser.yy"
                   { add_term((yyvsp[0].str)); }
#line 3973 "parser.cc"
    break;

  case 338:
#line 1158 "parser.yy"
                         { add_variables((yyvsp[0].strs), OBJECT_TYPE); }
#line 3979 "parser.cc"
    break;

  case 339:
#line 1159 "parser.yy"
                                   { add_variables((yyvsp[-1].strs), (yyvsp[0].type)); }
#line 3985 "parser.cc"
    break;

  case 341:
#line 1162 "parser.yy"
                        { (yyval.strs) = new std::vector<const std::string*>(1, (yyvsp[0].str)); }
#line 3991 "parser.cc"
    break;

  case 342:
#line 1163 "parser.yy"
                                     { (yyval.strs) = (yyvsp[-1].strs); (yyval.strs)->push_back((yyvsp[0].str)); }
#line 3997 "parser.cc"
    break;

  case 343:
#line 1166 "parser.yy"
                         {std::cout << "empty types line " << line_number << std::endl;}
#line 4003 "parser.cc"
    break;

  case 344:
#line 1167 "parser.yy"
                       { add_names((yyvsp[0].strs), OBJECT_TYPE); }
#line 4009 "parser.cc"
    break;

  case 345:
#line 1168 "parser.yy"
                                 { add_names((yyvsp[-1].strs), (yyvsp[0].type)); }
#line 4015 "parser.cc"
    break;

  case 347:
#line 1171 "parser.yy"
                { (yyval.strs) = new std::vector<const std::string*>(1, (yyvsp[0].str)); }
#line 4021 "parser.cc"
    break;

  case 348:
#line 1172 "parser.yy"
                         { (yyval.strs) = (yyvsp[-1].strs); (yyval.strs)->push_back((yyvsp[0].str)); }
#line 4027 "parser.cc"
    break;

  case 349:
#line 1175 "parser.yy"
                { std::cout << "--" << std::endl; require_typing(); }
#line 4033 "parser.cc"
    break;

  case 350:
#line 1175 "parser.yy"
                                                                           { (yyval.type) = (yyvsp[0].type); }
#line 4039 "parser.cc"
    break;

  case 351:
#line 1178 "parser.yy"
              { (yyval.type) = OBJECT_TYPE; }
#line 4045 "parser.cc"
    break;

  case 352:
#line 1179 "parser.yy"
                 { (yyval.type) = make_type((yyvsp[0].str)); }
#line 4051 "parser.cc"
    break;

  case 353:
#line 1180 "parser.yy"
                            { (yyval.type) = make_type(*(yyvsp[-1].types)); delete (yyvsp[-1].types); }
#line 4057 "parser.cc"
    break;

  case 354:
#line 1183 "parser.yy"
               { (yyval.types) = new TypeSet(); }
#line 4063 "parser.cc"
    break;

  case 355:
#line 1184 "parser.yy"
                  { (yyval.types) = new TypeSet(); (yyval.types)->insert(make_type((yyvsp[0].str))); }
#line 4069 "parser.cc"
    break;

  case 356:
#line 1185 "parser.yy"
                     { (yyval.types) = (yyvsp[-1].types); }
#line 4075 "parser.cc"
    break;

  case 357:
#line 1186 "parser.yy"
                        { (yyval.types) = (yyvsp[-1].types); (yyval.types)->insert(make_type((yyvsp[0].str))); }
#line 4081 "parser.cc"
    break;

  case 359:
#line 1196 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4087 "parser.cc"
    break;

  case 360:
#line 1199 "parser.yy"
                      { delete (yyvsp[0].str); }
#line 4093 "parser.cc"
    break;

  case 361:
#line 1202 "parser.yy"
                  { delete (yyvsp[0].str); }
#line 4099 "parser.cc"
    break;

  case 362:
#line 1205 "parser.yy"
            { delete (yyvsp[0].str); }
#line 4105 "parser.cc"
    break;

  case 363:
#line 1208 "parser.yy"
          { delete (yyvsp[0].str); }
#line 4111 "parser.cc"
    break;

  case 364:
#line 1211 "parser.yy"
          { delete (yyvsp[0].str); }
#line 4117 "parser.cc"
    break;

  case 365:
#line 1214 "parser.yy"
        { delete (yyvsp[0].str); }
#line 4123 "parser.cc"
    break;

  case 366:
#line 1217 "parser.yy"
              { delete (yyvsp[0].str); }
#line 4129 "parser.cc"
    break;

  case 367:
#line 1220 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4135 "parser.cc"
    break;

  case 368:
#line 1223 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4141 "parser.cc"
    break;

  case 369:
#line 1226 "parser.yy"
                              { delete (yyvsp[0].str); }
#line 4147 "parser.cc"
    break;

  case 370:
#line 1229 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4153 "parser.cc"
    break;

  case 371:
#line 1232 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4159 "parser.cc"
    break;

  case 372:
#line 1235 "parser.yy"
                        { delete (yyvsp[0].str); }
#line 4165 "parser.cc"
    break;

  case 373:
#line 1238 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4171 "parser.cc"
    break;

  case 374:
#line 1241 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4177 "parser.cc"
    break;

  case 375:
#line 1244 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4183 "parser.cc"
    break;

  case 376:
#line 1247 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4189 "parser.cc"
    break;

  case 377:
#line 1250 "parser.yy"
                      { delete (yyvsp[0].str); }
#line 4195 "parser.cc"
    break;

  case 378:
#line 1253 "parser.yy"
                      { delete (yyvsp[0].str); }
#line 4201 "parser.cc"
    break;

  case 379:
#line 1256 "parser.yy"
                { delete (yyvsp[0].str); }
#line 4207 "parser.cc"
    break;

  case 380:
#line 1259 "parser.yy"
                    { delete (yyvsp[0].str); }
#line 4213 "parser.cc"
    break;


#line 4217 "parser.cc"

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
#line 1288 "parser.yy"


/* Outputs an error message. */
static void yyerror(const std::string& s) {
	std::cerr << PACKAGE ":" << current_file << ':' << line_number << ": " << s << std::endl;
	success = false;
}


/* Outputs a warning. */
static void yywarning(const std::string& s) {
	if (warning_level > 0) {
		std::cerr << PACKAGE ":" << current_file << ':' << line_number << ": " << s << std::endl;
		if (warning_level > 1) {
			success = false;
		}
	}
}


/* Creates an empty domain with the given name. */
static void make_domain(const std::string* name) {
	domain = new Domain(*name);
	domains[*name] = domain;// 添加映射关系
	requirements = &domain->requirements;
	problem = NULL;
	delete name;
}



/* Creates an empty problem with the given name. */
static void make_problem(const std::string* name, const std::string* domain_name) {
	// 获取problem对应的domain是否存在
	std::map<std::string, Domain*>::const_iterator di = domains.find(*domain_name);
	if (di != domains.end()) {
		domain = (*di).second;
	}
	// domain不存在直接结束
	else {
		domain = new Domain(*domain_name);
		domains[*domain_name] = domain;
		yyerror("undeclared domain `" + *domain_name + "' used");
  	}
	// 获取domain的requirement
	requirements = new Requirements(domain->requirements);
	problem = new Problem(*name, *domain);
	my_problem = problem;
	// 判断是否需要reward
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
	if (!t.second)
	{
		t.first = domain->types().add_type(*name);
		
		if (name_kind != TYPE_KIND)
			yywarning("implicit declaration of type `" + *name + "'");
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
			if (problem != NULL)
				v.first = problem->terms().add_variable(OBJECT_TYPE);
			else
				v.first = domain->terms().add_variable(OBJECT_TYPE);

			context.insert(*name, v.first);
			yyerror("free variable `" + *name + "' used");
    	}
		delete name;
		return v.first;
	} 
	else {	
		TermTable& terms = (problem != NULL) ? problem->terms() : domain->terms();
		
		const PredicateTable& predicates = (parsing_obs_token ? domain->observables() : domain->predicates());
		
		std::pair<Object, bool> o = terms.find_object(*name);

		if (!o.second) {
			size_t n = term_parameters.size();
			if (parsing_atom && predicates.arity(atom_predicate) > n) {
				o.first = terms.add_object(*name, predicates.parameter(atom_predicate, n));
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
/* token.print(cout, ((Domain)problem->domain()).observables(),
    	problem->domain().functions(),
    	problem->terms()); */
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

    /* o.print(cout, ((Domain)problem->domain()).predicates(),
  	      problem->domain().functions(),
  	      problem->terms()); */

	/* token */
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
		yywarning("ignoring repeated declaration of action `" + action->name() + "'");
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
		yywarning("ignoring repeated declaration of event `" + action->name() + "'");
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

	while (is_variable(quantified[n])) n--;
	
	for (size_t i = n + 1; i <= m; i++) {
		qeffect->add_parameter(quantified[i]);
	}

	quantified.resize(n);
	return qeffect;
}


/* Adds an outcome to the given probabilistic effect.*/
static void add_effect_outcome(ProbabilisticEffect& peffect,
			        const Rational* p, const pEffect& effect) {
    std::cout << "*p = " << *p << std::endl; 
    // oneof涉及到non-deterministic效果
	if((*p == -1.0 || *p == -2.0 || *p == -3.0) && !requirements->non_deterministic){
		yywarning("assuming `:non-deterministic' requirement");
		requirements->non_deterministic = true;    
		/* requirements->probabilistic_effects = true; */
	}
    // 一般的probability effect
	else if ((*p != -1.0 && *p != -2.0 || *p != -3.0) && !requirements->probabilistic_effects) {
    	yywarning("assuming `:probabilistic-effects' requirement1");
		requirements->probabilistic_effects = true;
	} 
    
	if(*p == -1.0){ // okay, its an oneof nd-effect
	}
	else if(*p == -2.0){ // okay, its an unknown nd-effect
	}
	else if(*p == -3.0){ // okay, its an or nd-effect
	}
	else if (*p < 0.0 || *p > 1.0) {
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
		yywarning("assuming `:probabilistic-effects' requirement2");
		requirements->probabilistic_effects = true;
	}
	/* if (*p < 0 || *p > 1) {
		yyerror("outcome probability needs to be in the interval [0,1]");
	} */
	if (!peffect.add_foutcome(*p, effect)) {
		yyerror("effect outcome probabilities add up to more than 1");
	}
	/* cout << "done adding feffect" <<endl; */
	/* delete p; */
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
static void add_names(const std::vector<const std::string*>* names, Type type) {
	for (std::vector<const std::string*>::const_iterator si = names->begin(); si != names->end(); si++) {
		const std::string* s = *si;
		if (name_kind == TYPE_KIND) {// 1-if
			if (*s == OBJECT_NAME) {// 2-if
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
			}// end 2-if

		} else if (name_kind == CONSTANT_KIND) {// 1-if
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
				yywarning("ignoring declaration of object `" + *s + "' previously declared as constant");
			} else {
				std::pair<Object, bool> o = problem->terms().find_object(*s);
				if (!o.second) {
					problem->terms().add_object(*s, type);
				} else {
					TypeSet components;
					domain->types().components(components, problem->terms().type(o.first));
					components.insert(type);
					problem->terms().set_type(o.first, make_type(components));
				}
			}
		}// end 1-if
		delete s;
	}// end-for

	delete names;
}


/* Adds variables to the current variable list. */
static void add_variables(const std::vector<const std::string*>* names, Type type) {
	for (std::vector<const std::string*>::const_iterator si = names->begin(); si != names->end(); si++) {
    	const std::string* s = *si;// 获取名字
		// 是普通谓词
		if (parsing_predicate && !parsing_obs_token) {
			if (!repeated_predicate)
				domain->predicates().add_parameter(predicate, type);
		}
		// 函数
		else if (parsing_function && !parsing_obs_token ) {
			if (!repeated_function) {
				domain->functions().add_parameter(function, type);
			}
		}
		// 可观察谓词
		else if(parsing_predicate && parsing_obs_token){
			if (!repeated_function) {
				domain->observables().add_parameter(function, type);
			}
		}
		// term
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
	}// end-for
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
		if(parsing_obs_token)
			atom_predicate = domain->observables().add_predicate(*name);      
		else
			atom_predicate = domain->predicates().add_predicate(*name);
	
		undeclared_atom_predicate = true;
	
		if (problem != NULL)
			yywarning("undeclared predicate `" + *name + "' used");
		else
			yywarning("implicit declaration of predicate `" + *name + "'");
	} 
	else {
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
		if (problem != NULL)
			yywarning("undeclared function `" + *name + "' used");
		else 
			yywarning("implicit declaration of function `" + *name + "'");
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
    Term term = make_term(name);// 根据名字创建term
    // 获取term table
    const TermTable& terms = (problem != NULL) ? problem->terms() : domain->terms();
    // 当前正在解析atom
    if (parsing_atom) {
        // 根据是否解析obs判断是普通谓词还是可观察变量
        PredicateTable& predicates = (!parsing_obs_token ? domain->predicates() : domain->observables());
        size_t n = term_parameters.size();
        if (undeclared_atom_predicate) {
            predicates.add_parameter(atom_predicate, terms.type(term));
        } else if (predicates.arity(atom_predicate) > n 
                && !domain->types().subtype(terms.type(term),predicates.parameter(atom_predicate,n))) {
            yyerror("type mismatch");
        }
    // 当前在解析application
    } else if (parsing_application) {
        FunctionTable& functions = domain->functions();
        size_t n = term_parameters.size();
        if (undeclared_appl_function) {
            functions.add_parameter(appl_function, terms.type(term));
        } else if (functions.arity(appl_function) > n 
                && !domain->types().subtype(terms.type(term), functions.parameter(appl_function, n))) {
            yyerror("type mismatch");
        }
    }
  term_parameters.push_back(term);
}


/* Creates the atomic formula just parsed. */
static const Atom* make_atom() {
    size_t n = term_parameters.size();
    if(parsing_obs_token){// 当前正在解析observation
        // 参数个数情况判断
        if (domain->observables().arity(atom_predicate) < n) {
            yyerror("too many parameters passed to obs `"
                + domain->observables().name(atom_predicate) + "'");
        } else if (domain->observables().arity(atom_predicate) > n) {
            yyerror("too few parameters passed to obs `"
                + domain->observables().name(atom_predicate) + "'");
        }
    }
    // 当前解析普通atom
    else{
        // 参数个数情况判断
        if (domain->predicates().arity(atom_predicate) < n) {
            yyerror("too many parameters passed to predicate `"
                + domain->predicates().name(atom_predicate) + "'");
        } else if (domain->predicates().arity(atom_predicate) > n) {
            yyerror("too few parameters passed to predicate `"
                + domain->predicates().name(atom_predicate) + "'");
        }
    }
    parsing_atom = false;
    // 创建atom
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


/* Creates a subtraction.  term - opt_term */
static const Expression* make_subtraction(const Expression& term,
					  const Expression* opt_term) {
    if (opt_term != NULL) {// 两个数实现减法
        return new Subtraction(term, *opt_term);
    } else {// 一个数实现负数
        return new Subtraction(*new Value(0.0), term);
    }
}


/* Creates an atom or fluent for the given name to be used in an
   equality formula. */
static void make_eq_name(const std::string* name) {
    std::pair<Function, bool> f = domain->functions().find_function(*name);
    if (f.second) {// domain中定义的function
        prepare_application(name);
        eq_expr = make_application();
    } else {
        /* Assume this is a term. */
        eq_term = make_term(name);
        eq_expr = NULL;
    }
}


/* Creates an equality formula. 使用到了 =的前提条件 */
static const StateFormula* make_equality() {
    if (!requirements->equality) {
        yywarning("assuming `:equality' requirement");
        requirements->equality = true;
    }
    // 等号两侧表达式不为空
    if (first_eq_expr != NULL && eq_expr != NULL) {
        return new Comparison(Comparison::EQ_CMP, *first_eq_expr, *eq_expr);
    } else if (first_eq_expr == NULL && eq_expr == NULL) {// 两者均为空
        // 获取term表格
        const TermTable& terms = (problem != NULL) ? problem->terms() : domain->terms();
        // 判断类型是否相同
        if (domain->types().subtype(terms.type(first_eq_term), terms.type(eq_term))
            || domain->types().subtype(terms.type(eq_term),  terms.type(first_eq_term)))
        {
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
        // 需要否定的formual不是赋值或者比较，则需要支持析取式
        } else if (typeid(negand) != typeid(Equality) && typeid(negand) != typeid(Comparison)) {
            require_disjunction();
        }
    //创建并返回指针
    return &Negation::make_negation(negand);
}


/* Creates an implication. f1 -> f2 */
static const StateFormula* make_implication(const StateFormula& f1, const StateFormula& f2) {
    require_disjunction();// 判断是否有disjunction，没有则warning并开启
    Disjunction* disj = new Disjunction();//创建析取式
    disj->add_disjunct(Negation::make_negation(f1));//创建 !f1 \/ f2 
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
	
	while (is_variable(quantified[n])) n--;

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
    if (!requirements->rewards) {// goal中含有reward判断是否使用reward关键字
        yyerror("goal reward only allowed with the `:rewards' requirement");
    } else {
        const Application& reward_appl =
            Application::make_application(reward_function, TermList());// 创建一个application存储reward的操作
        const Assignment* reward_assignment =
            new Assignment(Assignment::INCREASE_OP, reward_appl, goal_reward);// 对该aplication赋值
        problem->set_goal_reward(*reward_assignment);//设置reward目标
    }
}


/* Sets the default metric for the current problem. */
static void set_default_metric() {
    if (requirements->rewards) {
        const Application& reward_appl =
            Application::make_application(reward_function, TermList());
        problem->set_metric(reward_appl);//设置评估函数
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
  
    const StateFormula &init = problem->init_formula();// 得到初始状态公式
    if(!&init)
        return;
    // 判断是否为和取式
    const Conjunction *c = dynamic_cast<const Conjunction*>(&init);
    if(c != NULL){
        // 将初始状态的每一个atom添加
        for (size_t i = 0; i < c->size(); i++) {
            const Atom *a = dynamic_cast<const Atom*>(&c->conjunct(i));
            if(a != NULL){// 普通atom直接添加
                problem->add_init_atom(*a);	
            }
            else{// oneof等形式的atom，递归处理
                make_all_dynamic(c->conjunct(i));
            }      
        }
    }
    else
        make_all_dynamic(init);
}
