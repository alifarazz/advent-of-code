(require '[clojure.string :as str])

(defn convert-depth [s]
  (defn get-sign-x [dir]
    (case dir
      "forward" 1
      0))
  (defn get-sign-y [dir]
    (case dir
      "down" 1
      "up" -1
      0))
  (let [dir-amnt (str/split s #" ")
        dir      (first dir-amnt)
        amnt     (->> (nth dir-amnt 1)
                      (Integer/parseInt))]
    (list (* (get-sign-x dir)
             amnt)
          (* (get-sign-y dir)
             amnt))))

(defn get-v []
  (->> (slurp "input")
       (str/split-lines)
       (map convert-depth)))

(defn p1 [v]
  (* (reduce + (map first v))
     (reduce + (map #(nth % 1) v))))

(defn p2 [v]
  (loop [v     v
         depth 0
         horiz 0
         aim   0]
    (if (empty? v)
      (* horiz depth)
      (let [xy (first v)
            x  (first xy)
            y  (nth xy 1)]
        (recur (rest v)
               (+ depth (* aim x))
               (+ horiz x)
               (+ aim y))))))

(time (let [v  (get-v)
            a1 (p1 v)
            a2 (p2 v)]
        (println a1 a2)))
