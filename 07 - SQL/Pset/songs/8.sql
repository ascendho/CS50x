-- The names of songs that feature other artists.
SELECT name
FROM songs
WHERE name LIKE '%feat.%';
