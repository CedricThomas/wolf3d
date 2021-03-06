/*
** disp_wall.c for wolf in /home/cedric/delivery/MUL/proj_wolf/wolf3d
** 
** Made by Cédric Thomas
** Login   <cedric@epitech.net>
** 
** Started on  Fri Dec 23 10:40:47 2016 Cédric Thomas
** Last update Thu Jan 12 14:29:26 2017 
*/
#include <math.h>
#include <SFML/Graphics.h>
#include "wolf.h"

int		on_wall(t_cast one, t_cast two)
{
  if (one.mcase.x == two.mcase.x && one.mcase.y != two.mcase.y
      && (one.mcase.y == two.mcase.y - 1 || one.mcase.y == two.mcase.y + 1))
    return (1);
  if (one.mcase.y == two.mcase.y && one.mcase.x != two.mcase.x
      && (one.mcase.x == two.mcase.x - 1 || one.mcase.x == two.mcase.x + 1))
    return (1);
  return (0);
}

int		intersect(int index, t_map *m, t_player *p)
{
  float         distfov;
  t_cast	prev;
  t_cast	act;
  t_cast	next;

  distfov = (float)(p->fov / WIDTH);
  prev = adv_raycast(*p, *m, ((index - 1) * distfov) - p->fov / 2 + p->angle);
  act = adv_raycast(*p, *m, (index * distfov) - p->fov / 2 + p->angle);
  next = adv_raycast(*p, *m, ((index + 1) * distfov) - p->fov / 2 + p->angle);
  if (prev.impact != act.impact || prev.impact != act.impact)
    return (1);
  if ((prev.dist - act.dist > act.dist * 10 / 100 && !on_wall(act, prev))
      || (next.dist - act.dist > act.dist * 10 / 100 && !on_wall(act, next)))
    return (1);
  return (0);
}

sfColor		getcolor(t_cast info, t_map *m, int index, t_player *p)
{
  sfColor	color;

  if (m->color)
    {
      if (info.impact == 0)
	color = sfRed;
      else if (info.impact == 1)
	color = sfBlue;
      else if (info.impact == 2)
	color = sfGreen;
      else if (info.impact == 3)
	color = sfWhite;
    }
  else
    if (info.impact <= 1)
      color = mycolor(255, 235, 149, 50);
    else
      color = mycolor(220, 235, 149, 50);
  if (intersect(index, m, p))
    color.a = 0;
  return (color);
}

void            draw_wall(t_map *m, t_player *p, t_pixelbuff *buff)
{
  int           k;
  t_cast        info;
  float         distfov;
  sfColor       color;

  k = -1;
  distfov = (float)(p->fov / WIDTH);
  while (++k <= WIDTH)
    {
      info = adv_raycast(*p, *m, (k * distfov) - p->fov / 2 + p->angle);
      info.height = info.dist /
	CASELEN * cosd((-1 * p->fov / 2) + k * distfov);
      info.height = (info.height < 1 ? HEIGHT : HEIGHT / info.height);
      color = getcolor(info, m, k, p);
      draw_col(buff, myvector2i(WIDTH - k,
				HEIGHT / 2 - info.height / 2), info.height, color);
      my_put_pixel(buff, WIDTH - k,
		   HEIGHT / 2 - info.height / 2, mycolor(0, 0, 0, 0));
    }
}
