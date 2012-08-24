#ifndef XFA_AST_HH
#define XFA_AST_HH

#include <memory>
#include <vector>

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
        assert(action_type == "fire" || action_type == "alert");
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

struct Range {
    int low, high;
    Range(int l, int h) : low(l), high(h) {}
};


struct Transition
{
    std::string source, dest;
    std::vector<Range> symbol_ranges;
    ActionList actions;

    Transition(std::string const & s, std::string const & d, std::vector<Range> const & rs)
        : source(s)
        , dest(d)
        , symbol_ranges(rs)
    {}

    Transition(std::string const & s, std::string const & d, std::vector<Range> const & rs, ActionList const & acts)
        : source(s)
        , dest(d)
        , symbol_ranges(rs)
        , actions(acts)
    {}
};

struct State
{
    std::string name;
    std::shared_ptr<Action> action;

    State(std::string const & n)
        : name(n)
    {}

    State(std::string const & n, Action * action_takes_ownership)
        : name(n)
        , action(action_takes_ownership)
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

#endif
