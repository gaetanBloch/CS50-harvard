SELECT DISTINCT(people.name)
  FROM people
       INNER JOIN stars
       ON stars.person_id = people.id
          AND people.name NOT LIKE "kevin bacon"

       INNER JOIN movies
       ON movies.id = stars.movie_id
          AND movies.id IN
          
              (SELECT movies.id
                FROM movies
                    INNER JOIN stars
                    ON stars.movie_id = movies.id

                    INNER JOIN people
                    ON people.id = stars.person_id
                        AND people.name LIKE "kevin bacon");