/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#pragma once

#include "t_generator.h"


class t_rdsn_generator : public t_generator 
{
public:
    t_rdsn_generator(t_program* program,
        const std::map<std::string, std::string>& parsed_options,
        const std::string& option_string);

	~t_rdsn_generator(void);

private:
    // possibly changed compared to cpp_generator
    virtual void init_generator();    
    virtual void close_generator();
    virtual void generate_typedef(t_typedef* ttypedef);
    virtual void generate_enum(t_enum* tenum);
    virtual void generate_struct(t_struct* tstruct);

    // always changed compared to cpp_generator
    virtual void generate_service(t_service* tservice);

private:
    std::string get_full_type_name(t_type* t);

private:
    std::ofstream _f_php;
};

