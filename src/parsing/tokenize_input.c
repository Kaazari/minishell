/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Gère l'expansion des variables d'environnement ($VAR)
 *
 * Cette fonction :
 * - Prépare la structure var_expansion avec les données nécessaires
 * - Appelle add_variable_to_word pour traiter l'expansion
 * - Met à jour l'index de position dans l'entrée
 *
 * @param input: Ligne de commande complète
 * @param i: Index de position dans l'entrée (mis à jour)
 * @param data: Structure de données de tokenization
 * @param shell: Structure shell pour accéder à l'environnement
 *
 * Note: Cette fonction n'est appelée que si on n'est pas dans des quotes simples
 * car les variables ne sont pas expansées dans les quotes simples
 */
static void	handle_variable_expansion(char *input, int *i, t_token_data *data,
		t_shell *shell)
{
	t_var_expansion	var_exp;

	var_exp.input = input;
	var_exp.i = *i;
	var_exp.word = data->current_word;
	var_exp.pos = data->word_pos;
	*i = add_variable_to_word(&var_exp, shell);
	(*i)++;
}

/**
 * Traite les caractères spéciaux selon le contexte
 *
 * Cette fonction gère différents types de caractères :
 * - Redirections (> < >> <<) : crée des tokens de redirection
 * - Opérateurs logiques (|| &&) : crée des tokens logiques
 * - Pipes (|) : crée des tokens de pipe
 * - Séparateurs (espaces, tabulations) : termine le mot courant
 * - Caractères normaux : ajoute au mot courant
 * - Quotes : ignore (gérées ailleurs)
 *
 * @param input: Ligne de commande complète
 * @param i: Index de position dans l'entrée (mis à jour)
 * @param data: Structure de données de tokenization
 * @param quote_state: État actuel des quotes (0 = pas de quotes)
 *
 * Note: Les caractères spéciaux ne sont traités que si on n'est pas dans des quotes
 */
static void	handle_special_chars(char *input, int *i, t_token_data *data,
		char quote_state)
{
	if ((input[*i] == '>' || input[*i] == '<') && !quote_state)
		handle_redirection_token(data, input, i);
	else if (input[*i] == '|' && input[*i + 1] == '|' && !quote_state)
		handle_logical_token(data, input, i);
	else if (input[*i] == '&' && input[*i + 1] == '&' && !quote_state)
		handle_logical_token(data, input, i);
	else if (input[*i] == '|' && !quote_state)
		handle_pipe_token(data, i);
	else if (is_separator(input[*i]) && !quote_state)
		handle_separator_token(data, i);
	else if (!is_quote(input[*i]))
	{
		add_char_to_word(data->current_word, data->word_pos, input[*i]);
		(*i)++;
	}
	else
		(*i)++;
}

/**
 * Traite chaque caractère de l'entrée pour la tokenization
 *
 * Cette fonction est le cœur de la tokenization et gère :
 * - L'expansion des variables d'environnement ($VAR)
 * - Les quotes simples et doubles avec leur contenu
 * - Les quotes vides (ignorées complètement)
 * - L'échappement des backslashes dans les quotes
 * - Les caractères spéciaux (redirections, pipes, opérateurs logiques)
 * - Les erreurs de quotes non fermées
 *
 * Comportement des quotes :
 * - Les quotes vides sont complètement ignorées
 * - Les variables sont expansées dans les quotes doubles, pas dans les simples
 * - Les backslashes échappent les caractères dans les quotes
 *
 * @param input: Ligne de commande à traiter
 * @param data: Structure de données de tokenization
 * @param quote_state: État des quotes (mis à jour)
 * @param shell: Structure shell pour l'expansion des variables
 *
 * Note: Cette fonction affiche des messages d'erreur pour les quotes non fermées
 * et réinitialise la position du mot pour éviter les tokens invalides
 */
void	process_input_chars(char *input, t_token_data *data, char *quote_state,
		t_shell *shell)
{
	int	i;
	int	quote_start;

	i = 0;
	quote_start = -1;
	while (input[i])
	{
		if (input[i] == '$' && *quote_state != '\'')
		{
			handle_variable_expansion(input, &i, data, shell);
			continue ;
		}
		if (is_quote(input[i]))
		{
			if (*quote_state == 0)
			{
				*quote_state = input[i];
				quote_start = i;
				i++;
			}
			else if (*quote_state == input[i])
			{
				if (quote_start == i - 1)
				{
					*quote_state = 0;
					quote_start = -1;
					i++;
					continue ;
				}
				*quote_state = 0;
				quote_start = -1;
				i++;
			}
			else
			{
				add_char_to_word(data->current_word, data->word_pos, input[i]);
				i++;
			}
			continue ;
		}
		if (input[i] == '\\' && *quote_state != 0)
		{
			i++;
			if (input[i])
			{
				add_char_to_word(data->current_word, data->word_pos, input[i]);
				i++;
			}
			continue ;
		}
		handle_special_chars(input, &i, data, *quote_state);
	}
	if (*quote_state != 0)
	{
		if (*quote_state == '\'')
			write(STDERR_FILENO, "minishell: Error: Unmatched single quotes\n", 42);
		else
			write(STDERR_FILENO, "minishell: Error: Unmatched double quotes\n", 42);
		*(data->word_pos) = 0;
	}
}
