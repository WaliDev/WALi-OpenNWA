#ifndef XFA_AST_HH
#define XFA_AST_HH

#include <memory>
#include <vector>
#include <string>
#include <cassert>

namespace xfa_parser {

struct Command;

struct Action {
    int rule_id;
    std::string action_type;
    int operand_id;
    std::shared_ptr<Command> command;

    Action(int rid, std::string const & ty)
        : rule_id(rid)
        , action_type(ty)
        , operand_id(-1)
    {
        assert(action_type == "fire" || action_type == "alert" || action_type == "reject");
    }

    Action(int rid, std::string const & ty, int opid, Command * cmd_takes_ownership)
        : rule_id(rid)
        , action_type(ty)
        , operand_id(opid)
        , command(cmd_takes_ownership)
    {}
};

struct Command {
    std::string name;
    std::vector<std::string> arguments;
    std::shared_ptr<Action> consequent, alternative;

    Command(std::string const & n, std::vector<std::string> const & args)
        : name(n)
        , arguments(args)
    {}

    Command(std::string const & n,
            Action * cons_takes_ownership,
            Action * alt_takes_ownership)
        : name(n)
        , consequent(cons_takes_ownership)
        , alternative(alt_takes_ownership)
    {}

    Command(std::string const & n,
            std::vector<std::string> const & args,
            Action * cons_takes_ownership)
        : name(n)
        , arguments(args)
        , consequent(cons_takes_ownership)
    {}

    Command(std::string const & n,
            Action * cons_takes_ownership)
        : name(n)
        , consequent(cons_takes_ownership)
    {}

    Command(std::string const & n,
            std::vector<std::string> const & args,
            Action * cons_takes_ownership,
            Action * alt_takes_ownership)
        : name(n)
        , arguments(args)
        , consequent(cons_takes_ownership)
        , alternative(alt_takes_ownership)
    {}
};

typedef std::vector<std::shared_ptr<Action> > ActionList;

struct Symbol {
    virtual ~Symbol() {}
};

struct Range : Symbol {
    int low, high;
};

inline Range* make_range(int l, int h) {
    Range * r = new Range();
    r->low = l;
    r->high = h;
    return r;
}

struct Name : Symbol {
    std::string name;
};

inline Name * make_name(std::string const & name) {
    Name * n = new Name();
    n->name = name;
    return n;
}

struct Transition
{
    std::string source, dest;
    std::vector<std::unique_ptr<Symbol>> symbols;
    ActionList actions;

    Transition(std::string const & s, std::string const & d, std::vector<std::unique_ptr<Symbol>> & rs_moves_from)
        : source(s)
        , dest(d)
        , symbols(std::move(rs_moves_from))
    {}

    Transition(std::string const & s, std::string const & d, std::vector<std::unique_ptr<Symbol>> & rs_moves_from, ActionList const & acts)
        : source(s)
        , dest(d)
        , symbols(std::move(rs_moves_from))
        , actions(acts)
    {}
};

struct State
{
    std::string name;
    std::shared_ptr<ActionList> actions;

    State(std::string const & n)
        : name(n)
    {}

    State(std::string const & n, ActionList * action_takes_ownership)
        : name(n)
        , actions(action_takes_ownership)
    {}
};

struct Xfa
{
    std::vector<std::shared_ptr<State> > states;
    std::vector<std::shared_ptr<Transition> > transitions;
    std::string start_state;

    void add_state(State * st_takes_ownership) {
        states.push_back(std::shared_ptr<State>(st_takes_ownership));
    }

    void add_transition(Transition * tr_takes_ownership) {
        transitions.push_back(std::shared_ptr<Transition>(tr_takes_ownership));
    }
};

extern Xfa * parse_stdin();

}

#endif
