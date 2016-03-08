#include "t_rdsn_generator.h"
#include "platform.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <exception>

t_rdsn_generator::t_rdsn_generator(t_program* program,
                    const std::map<std::string, std::string>& parsed_options,
                    const std::string& option_string)
                  : t_generator(program)
{
}

t_rdsn_generator::~t_rdsn_generator(void)
{
}

void t_rdsn_generator::close_generator()
{
    _f_php << "?>" << std::endl;
    _f_php.close();
}

void t_rdsn_generator::init_generator()
{
    // Make output directory
    MKDIR(get_out_dir().c_str());

    std::string f_php_name = get_out_dir() + program_name_ + ".php";
    _f_php.open(f_php_name.c_str());
    _f_php << "<?php " << std::endl;
    _f_php << "$_PROG = new t_program(\"" << program_name_ << "\");" << std::endl;
    _f_php << std::endl;
    for (auto& nm : get_program()->get_namespaces())
    {
        _f_php << "$_PROG->namespaces[\"" << nm.first << "\"] = \"" << nm.second << "\";" << std::endl;
    }
    _f_php << std::endl;
    for (auto& ic : get_program()->get_includes())
    {
        _f_php << "$tmp = new t_program(\"" << ic->get_name() << "\");" << std::endl;
        _f_php << "$_PROG->includes[$tmp->name] = $tmp;" << std::endl;
        for (auto& nm : ic->get_namespaces())
        {
            _f_php << "$tmp->namespaces[\"" << nm.first << "\"] = \"" << nm.second << "\";" << std::endl;
        }
        _f_php << std::endl;
    }
}

void t_rdsn_generator::generate_typedef(t_typedef* ttypedef)
{
    _f_php << "new t_typedef($_PROG, \"" << get_full_type_name(ttypedef->get_type()) << "\", \"" << ttypedef->get_symbolic() << "\");" << std::endl;
    _f_php << std::endl;
}

std::string t_rdsn_generator::get_full_type_name(t_type* ttype)
{
    if (ttype->is_base_type()) {
        return ttype->get_name();
    }

    if (ttype->is_container()) {
        if (ttype->is_map()) {
            t_map* tmap = (t_map*)ttype;
            return std::string("map< ") + get_full_type_name(tmap->get_key_type())
                + ", " + get_full_type_name(tmap->get_val_type()) + ">";
        }
        else if (ttype->is_set()) {
            t_set* tset = (t_set*)ttype;
            return std::string("set< ") + get_full_type_name(tset->get_elem_type()) + ">";
        }
        else if (ttype->is_list()) {
            t_list* tlist = (t_list*)ttype;
            return std::string("vector< ") + get_full_type_name(tlist->get_elem_type()) + ">";
        }
        else
        {
            throw std::string("unknown container type:") + ttype->get_name();
        }
    }
    else
    {
        return ttype->get_program()->get_name() + "." + ttype->get_name();
    }    
}

void t_rdsn_generator::generate_enum(t_enum* tenum)
{
    std::vector<t_enum_value*> constants = tenum->get_constants();

    _f_php << "$tmp = new t_enum($_PROG, \"" << tenum->get_name() << "\");" << std::endl;
    for (auto& c : constants)
    {
        _f_php << "$tmp->add_value(\"" << c->get_name() << "\", " << c->get_value() << ");" << std::endl;
    }
    _f_php << std::endl;
}

void t_rdsn_generator::generate_struct(t_struct* tstruct)
{
    _f_php << "$tmp = new t_struct($_PROG, \"" << tstruct->get_name() << "\");" << std::endl;
    for (auto& f : tstruct->get_members())
    {
        _f_php << "$tmp->add_field(\"" << f->get_name() << "\", \"" << get_full_type_name(f->get_type()) << "\");" << std::endl;
    }
    _f_php << std::endl;
}

void t_rdsn_generator::generate_service(t_service* svc)
{
    std::string svcname = svc->get_name();

    _f_php << "$tmp = new t_service($_PROG, \"" << svc->get_name() << "\");" << std::endl;
    for (auto& f : svc->get_functions())
    {
        _f_php << "$tmp2 = $tmp->add_function(\"" << get_full_type_name(f->get_returntype()) << "\", \"" << f->get_name() << "\");" << std::endl;
        for (auto& p : f->get_arglist()->get_members())
            _f_php << "$tmp2->add_param(\"" << p->get_name() << "\", \"" << get_full_type_name(p->get_type()) << "\");" << std::endl;
    }
    _f_php << std::endl;
}

/* register this generator with the main program */
THRIFT_REGISTER_GENERATOR(rdsn, "rDSN", 
    "    Generate service type and defintion file (.php) for project \n"
    "    'Robust Distributed System Nucleus (rDSN)'.\n"
    )
