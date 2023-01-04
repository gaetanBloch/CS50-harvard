SELECT movies.title, ratings.rating
  FROM movies
       INNER JOIN ratings
       ON ratings.movie_id = movies.id
          AND movies.year = 2010
 ORDER BY ratings.rating DESC, movies.title;