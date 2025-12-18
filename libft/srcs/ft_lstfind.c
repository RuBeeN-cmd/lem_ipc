#include "list.h"

void	*ft_lstfind(t_list *lst, void *to_find, int (*f)(void *, void *))
{
	while (lst)
	{
		if (f(lst->content, to_find))
			return (lst->content);
		lst = lst->next;
	}
	return (NULL);
}
