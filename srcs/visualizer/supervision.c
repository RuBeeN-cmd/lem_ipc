#include <visualizer/visualizer.h>

static void	create_supervision_panel_text_lines(t_visualizer *v, t_panel *panel, t_supervised_infos target_infos)
{
	if (panel->text_line_list)
		destroy_text_line_list(panel);
	panel->text_line_list = NULL;
	add_text_line(v->text_engine, v->font, panel, "Target Info : ", color_from_u32(0xFF000000), JUSTIFY_CENTER, 0);
	add_text_line(v->text_engine, v->font, panel, "Position : ", color_from_u32(0xFF000000), JUSTIFY_LEFT, 1);
	char *str = "(";
	str = ft_strjoin_free(str, ft_itoa(target_infos.pos.x), 2);
	str = ft_strjoin_free(str, ", ", 1);
	str = ft_strjoin_free(str, ft_itoa(target_infos.pos.y), 3);
	str = ft_strjoin_free(str, ")", 1);
	add_text_line(v->text_engine, v->font, panel, str, color_from_u32(0xFF000000), JUSTIFY_RIGHT, 1);
	free(str);

	add_text_line(v->text_engine, v->font, panel, "Team : ", color_from_u32(0xFF000000), JUSTIFY_LEFT, 2);
	str = ft_itoa(target_infos.team);
	add_text_line(v->text_engine, v->font, panel, str, get_team_color(target_infos.team), JUSTIFY_RIGHT, 2);
	free(str);

	add_text_line(v->text_engine, v->font, panel, "Alive : ", color_from_u32(0xFF000000), JUSTIFY_LEFT, 3);
	str = target_infos.is_alive ? "Yes" : "No";
	t_color alive_color = target_infos.is_alive ? color_from_u32(0xFF00FF00) : color_from_u32(0xFF0000FF);
	add_text_line(v->text_engine, v->font, panel, str, alive_color, JUSTIFY_RIGHT, 3);
}

void	update_supervision(t_visualizer *v)
{
	if (check_msg(v->ipc.msg_id, &v->target_infos, sizeof(v->target_infos), TARGET_INFOS_CHANNEL) != 1)
		return ;
	if (v->supervision_panel.visible == 0)
		v->supervision_panel.visible = 1;
	create_supervision_panel_text_lines(v, &v->supervision_panel, v->target_infos);
}