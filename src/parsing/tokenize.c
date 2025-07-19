/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* All functions have been moved to:
 * - tokenize_helpers.c: is_separator, is_quote, update_quote_state,
 *   add_char_to_word, save_word
 * - tokenize_env.c: get_env_value, add_variable_to_word
 * - tokenize_cmd.c: create_cmd, tokenize_words and related functions
 */
