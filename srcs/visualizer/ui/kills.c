#include <visualizer/visualizer.h>

static int	is_same_team(void *node, void *to_find)
{
	return (((t_kill_number *) node)->team == ((t_kill_number *) to_find)->team);
}

static int	cmp_kill_number(void *kill_number1, void *kill_number2) {
	return (((t_kill_number *) kill_number2)->number - ((t_kill_number *) kill_number1)->number);
}

static void	update_kill_list(t_visualizer *v)
{
	t_kill_info	kill_info;
	while (check_msg(v->ipc.msg_id, &kill_info, sizeof(t_kill_info), KILL_CHANNEL) == 1) {
		uint32_t killer_team = kill_info.killer_team;
		t_kill_number kn = {.number = 0, .team = killer_team};
		t_kill_number *found = NULL;
		if ((found = ft_lstfind(v->kills, &kn, is_same_team))) {
			found->number++;
		} else {
			t_kill_number *counter = malloc(sizeof(t_kill_number));
			counter->number = 1;
			counter->team = killer_team;
			t_list *new_node = ft_lstnew(counter);
			ft_lstadd_back(&v->kills, new_node);
		}
		ft_lstsort(v->kills, cmp_kill_number);
	}
}

static void	create_kills_panel_text_lines(t_visualizer *v, t_panel *panel)
{
	if (panel->text_line_list)
		destroy_text_line_list(panel);

	add_text_line(v->sdl.text_engine, v->sdl.font, panel, "Leaderboard: ", color_from_u32(0xFF000000), JUSTIFY_CENTER, 0);
	t_list *current = v->kills;
	for (size_t i = 0; i < MAX_LEADERBOARD && current; i++, current = current->next)
	{
		t_kill_number *kill_number = (t_kill_number *) current->content;
		if (kill_number)
		{
			char *line_text = ft_strjoin_free("Team ", ft_itoa(kill_number->team), 2);
			line_text = ft_strjoin_free(line_text, " : ", 1);
			add_text_line(v->sdl.text_engine, v->sdl.font, panel, line_text, get_team_color(kill_number->team), JUSTIFY_LEFT, i + 1);
			free(line_text);
			char *num_str = ft_itoa(kill_number->number);
			add_text_line(v->sdl.text_engine, v->sdl.font, panel, num_str, get_team_color(kill_number->team), JUSTIFY_RIGHT, i + 1);
			free(num_str);
		}
	}
}

void update_kills(t_visualizer *v)
{
	update_kill_list(v);
	create_kills_panel_text_lines(v, &v->kills_panel);
}