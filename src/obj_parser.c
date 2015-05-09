
#include <stdio.h>
#include <stdlib.h>
#include "core.h"

void				init_object(t_object *o)
{
	o->vertices_size = 0;
	o->indices_size = 0;
	o->vertices = NULL;
	o->colors = NULL;
	o->indices = NULL;
}

int					alloc_object(t_object *o)
{
	if (o->vertices_size > 0)
	{
		if (!(o->vertices =
			(GLfloat *)malloc(sizeof(GLfloat) * o->vertices_size * 3)))
			return (print_error("Failed to allocate vertices !\n", 0));
	}
	if (o->indices_size > 0)
	{
		if (!(o->indices =
			(GLushort *)malloc(sizeof(GLushort) * o->indices_size * 3)))
			return (print_error("Failed to allocate indices !\n", 0));
		if (!(o->colors =
			(GLfloat *)malloc(sizeof(GLfloat) * o->indices_size * 3)))
			return (print_error("Failed to allocate colors !\n", 0));
		if (!(o->texture_coord =
			(GLfloat *)malloc(sizeof(GLfloat) * o->indices_size * 6)))
			return (print_error("Failed to allocate texture coordinates !\n", 0));
	}
	return (1);
}

void				release_object(t_object *o)
{
	if (o->vertices != NULL)
		free(o->vertices);
	if (o->indices != NULL)
		free(o->indices);
}

int					get_buffer_next_line_size(char *file, int *offset)
{
	int				size;

	size = 0;
	while (file[*offset + size] != '\n')
	{
		if (!file[*offset + size])
			return (-1);
		size++;
	}
	return (size);
}

char				*get_buffer_next_line(char *file, int *file_size,
										int *offset, int *line_size)
{
	char			*line;
	int				i;

	if (offset >= file_size)
		return (NULL);
	if ((*line_size = get_buffer_next_line_size(file, offset)) == -1)
		return (NULL);
	if (!(line = (char *)malloc(sizeof(char) * *line_size + 1)))
		return (print_error_p("Failed to allocate memory !\n"));
	i = 0;
	while (file[*offset + i] != '\n' && file[*offset + i] != '\0')
	{
		line[i] = file[*offset + i];
		i++;
	}
	line[*line_size] = '\0';
	*offset += *line_size + 1;
	return (line);
}

int					parse_object_data(char *file, int *file_size, t_object *o)
{
	int						index;
	char					*line;
	int						size;
	int						r;
	int						i[4];
	int						j[3];
	GLfloat					grey;
	static GLfloat const	grey_lim[3] = { 0.2f, 0.2f, 0.8f };

	(void)o;
	index = 0;
	line = NULL;
	j[0] = 0;
	j[1] = 0;
	j[2] = 0;
	grey = grey_lim[0];
	while ((line = get_buffer_next_line(file, file_size, &index, &size)) != NULL)
	{
		if (size > 1)
		{
			if (!sncmp(line, "v ", 2))
			{
				sscanf(line, "v %f %f %f", &o->vertices[j[0]],
					&o->vertices[j[0] + 1], &o->vertices[j[0] + 2]);
				j[0] += 3;
			}
			else if (!sncmp(line, "f ", 2))
			{
				r = sscanf(line, "f %d %d %d %d", &i[0], &i[1], &i[2], &i[3]);
				if (r == 4)
				{
					o->indices[j[1] + 0] = i[0] - 1;
					o->indices[j[1] + 1] = i[1] - 1;
					o->indices[j[1] + 2] = i[2] - 1;
					o->indices[j[1] + 3] = i[0] - 1;
					o->indices[j[1] + 4] = i[2] - 1;
					o->indices[j[1] + 5] = i[3] - 1;
					o->colors[j[1] + 0] = grey;
					o->colors[j[1] + 1] = grey;
					o->colors[j[1] + 2] = grey;
					o->colors[j[1] + 3] = grey;
					o->colors[j[1] + 4] = grey;
					o->colors[j[1] + 5] = grey;

					o->texture_coord[j[2] + 0] = 0.0f;
					o->texture_coord[j[2] + 1] = 0.0f;
					o->texture_coord[j[2] + 2] = 1.0f;
					o->texture_coord[j[2] + 3] = 0.0f;
					o->texture_coord[j[2] + 4] = 0.0f;
					o->texture_coord[j[2] + 5] = 1.0f;

					o->texture_coord[j[2] + 6] = 1.0f;
					o->texture_coord[j[2] + 7] = 1.0f;
					o->texture_coord[j[2] + 8] = 0.0f;
					o->texture_coord[j[2] + 9] = 0.0f;
					o->texture_coord[j[2] + 10] = 1.0f;
					o->texture_coord[j[2] + 11] = 0.0f;
					j[2] += 12;
					j[1] += 6;
				}
				else if (r == 3)
				{
					o->indices[j[1] + 0] = i[0] - 1;
					o->indices[j[1] + 1] = i[1] - 1;
					o->indices[j[1] + 2] = i[2] - 1;
					o->colors[j[1] + 0] = grey;
					o->colors[j[1] + 1] = grey;
					o->colors[j[1] + 2] = grey;
					o->texture_coord[j[2] + 0] = 0.0f;
					o->texture_coord[j[2] + 1] = 0.0f;
					o->texture_coord[j[2] + 2] = 1.0f;
					o->texture_coord[j[2] + 3] = 0.0f;
					o->texture_coord[j[2] + 4] = 0.0f;
					o->texture_coord[j[2] + 5] = 1.0f;
					j[2] += 6;
					j[1] += 3;
				}
				else
				{
					o->indices[j[1] + 0] = 0.0f;
					o->indices[j[1] + 1] = 0.0f;
					o->indices[j[1] + 2] = 0.0f;
					j[1] += 3;
				}
				grey += grey_lim[1];
				if (grey >= grey_lim[2])
					grey = grey_lim[0];
			}
		}
		free(line);
		line = NULL;
	}
	return (1);
}

void				count_object_data(char *file, int *file_size, t_object *o)
{
	int				index;
	char			*line;
	int				size;
	int				r;
	int				t;

	index = 0;
	line = NULL;
	while ((line = get_buffer_next_line(file, file_size, &index, &size)))
	{
		if (!sncmp(line, "v ", 2))
			o->vertices_size++;
		else if (!sncmp(line, "f ", 2))
		{
			r = sscanf(line, "f %d %d %d %d", &t, &t, &t, &t);
			if (r == 4)
				o->indices_size += 2;
			if (r == 3)
				o->indices_size++;
		}
		free(line);
		line = NULL;
	}
}

int					parse_object(char const *filename, t_object *o)
{
	int				file_size;
	char			*file;

	init_object(o);
	if (!(file = read_file(filename)))
		return (0);
	file_size = slen(file);
	count_object_data(file, &file_size, o);
	dprintf(2, "vertices: %d, indices: %d\n", o->vertices_size, o->indices_size);
	if (!alloc_object(o))
		return (0);
	parse_object_data(file, &file_size, o);
	free(file);
	return (1);
}
