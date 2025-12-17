#include <visualizer/visualizer.h>

void	update_kill_list(t_visualizer *v)
{
	t_kill_info	kill_info;
	while (check_msg(v->ipc.msg_id, &kill_info, sizeof(t_kill_info), KILL_CHANNEL) == 1) {
		t_kill_info	*new_kill_info = malloc(sizeof(t_kill_info));
		if (!new_kill_info)
			continue;
		*new_kill_info = kill_info;
		DBG("Visualizer received kill info: killed_team=%d, killer_team=%d\n", kill_info.killed_team, kill_info.killer_team);
		t_list *new_node = ft_lstnew(new_kill_info);
		if (!new_node)
		{
			free(new_kill_info);
			continue;
		}
		ft_lstadd_back(&v->kills, new_node);
	}
}

static uint32_t *count_all_teams_kills(t_list *kills)
{
	static uint32_t team_kills[MAX_TEAMS] = {0};
	t_list *current = kills;
	while (current)
	{
		t_kill_info *kill_info = (t_kill_info *)current->content;
		if (kill_info)
		{
			team_kills[kill_info->killer_team]++;
		}
		current = current->next;
	}
	return (team_kills);
}

void	create_kills_panel_text_lines(t_visualizer *v, t_panel *panel)
{
	if (panel->text_line_list)
		destroy_text_line_list(panel);
	panel->text_line_list = NULL;
	add_text_line(v->text_engine, v->font, panel, "Kills : ", color_from_u32(0xFF000000), JUSTIFY_CENTER, 0);
	int	line_number = 1;
	uint32_t *team_kills = count_all_teams_kills(v->kills);
	for (uint32_t team_id = 1; team_id < MAX_TEAMS; team_id++)
	{
		if (team_kills[team_id] > 0)
		{
			char *str = ft_itoa(team_kills[team_id]);
			char *line_text = ft_strjoin_free("Team ", ft_itoa(team_id), 2);
			line_text = ft_strjoin_free(line_text, " : ", 1);
			add_text_line(v->text_engine, v->font, panel, line_text, get_team_color(team_id), JUSTIFY_LEFT, line_number);
			add_text_line(v->text_engine, v->font, panel, str, get_team_color(team_id), JUSTIFY_RIGHT, line_number);
			free(str);
			free(line_text);
			line_number++;
		}
	}
}