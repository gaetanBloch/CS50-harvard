SELECT AVG(energy) As average_energy FROM songs
JOIN artists
ON songs.artist_id = artists.id
WHERE artists.name LIKE "drake";