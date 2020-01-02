#ifndef PARSE_H
#define PARSE_H

void            test_parse();
t_node          *parse_commands();
t_redirection   *here_doc(t_token_kind kind);
t_redirection   *input_redirection(t_token_kind kind);
t_redirection   *output_aggregate();
t_redirection   *output_redirection(t_token_kind kind);
t_redirection   *here_string();
t_redirection   *input_aggregate();
const char      *redirect_name(t_token_kind kind);
// t_node *init_parse();

#endif